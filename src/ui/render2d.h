#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

#include "shaders/shader.h"

class Render2D
{
    struct Character {
        GLuint     texture_ID; // ID handle of the glyph texture
        glm::ivec2 size;       // Size of glyph
        glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
        GLuint     advance;    // Offset to advance to next glyph
    };

public:
    void setup(int width, int height);
    //void update_projection;
    void render_text(Shader *shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
    void render_rect(Shader *shader, GLfloat x, GLfloat y, GLfloat width, GLfloat height, glm::vec3 color, GLuint texture_ID);
	void render_rectP(Shader *shader, GLfloat x, GLfloat y, GLfloat width, GLfloat height, glm::vec3 color, GLuint texture_ID);
private:
    void setup_glyphs();

    std::map<GLchar, Character> characters;
    GLuint vao, vbo;
    glm::mat4 projection;
	int width, height;
};
