#include "render2d.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Render2D::setup_text(int width, int height)
{
	// TODO: replace with event system callback
	this->width = width;
	this->height = height;
	projection_text = glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height));

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font ("face")
	FT_Face face;
	if (FT_New_Face(ft, "assets/fonts/DejaVuSerif.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Width and height params. Width is 0 for dynamic resizing.
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Set active glyph to a character
	// FT_LOAD_RENDER flag creates 8-bit grayscale bitmap image (access by face->glyph->bitmap)
	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
				);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(GLuint) face->glyph->advance.x
		};
		characters.insert(std::pair<GLchar, Character>(c, character));
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &VAO_text);
	glGenBuffers(1, &VBO_text);
	glBindVertexArray(VAO_text);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_text);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Render2D::render_text(Shader *shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	// Activate corresponding render state
	shader->use();
	glUniform3f(glGetUniformLocation(shader->shader_id, "baseColor"), color.x, color.y, color.z);
	glUniformMatrix4fv(glGetUniformLocation(shader->shader_id, "projection"), 1, GL_FALSE, glm::value_ptr(projection_text));
	glUniform1i(glGetUniformLocation(shader->shader_id, "hasTexture"), true);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO_text);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = characters[*c];

		GLfloat xpos = x + ch.bearing.x * scale;
		GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.texture_ID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO_text);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Render2D::render_rect(Shader *shader, GLfloat x, GLfloat y, GLfloat width, GLfloat height, glm::vec3 color, GLuint texture_ID)
{
	// Send uniforms to the text shader
	shader->use();
	glUniform3f(glGetUniformLocation(shader->shader_id, "baseColor"), color.x, color.y, color.z);
	glUniformMatrix4fv(glGetUniformLocation(shader->shader_id, "projection"), 1, GL_FALSE, glm::value_ptr(projection_text));
	
	if (texture_ID == 0)
		glUniform1i(glGetUniformLocation(shader->shader_id, "hasTexture"), false);
	else
		glUniform1i(glGetUniformLocation(shader->shader_id, "hasTexture"), true);

	// Set active arrays
	glBindVertexArray(VAO_text);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_ID);

	// Update VBO for this rect
	GLfloat vertices[6][4] = {
		{ x,     	 y + height,   0.0, 0.0 },
		{ x,     	 y,       	   0.0, 1.0 },
		{ x + width, y,       	   1.0, 1.0 },

		{ x,         y + height,   0.0, 0.0 },
		{ x + width, y,       	   1.0, 1.0 },
		{ x + width, y + height,   1.0, 0.0 }
	};
	// Update content of VBO memory
	glBindBuffer(GL_ARRAY_BUFFER, VBO_text);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Render quad
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Render2D::render_rectP(Shader *shader, GLfloat x, GLfloat y, GLfloat width, GLfloat height, glm::vec3 color, GLuint texture_ID)
{
	render_rect(shader, x * this->width, y * this->height, width * this->width, height * this->height, color, texture_ID);
}
