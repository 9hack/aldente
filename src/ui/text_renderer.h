#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

#include "shaders/shader.h"

struct Character {
    GLuint     texture_ID; // ID handle of the glyph texture
    glm::ivec2 size;       // Size of glyph
    glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
    GLuint     advance;    // Offset to advance to next glyph
};

class TextRenderer
{
public:
    void setup(int width, int height);
    void render_text(Shader *shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
private:
    std::map<GLchar, Character> characters;
    GLuint VAO, VBO;
	glm::mat4 projection;
};

