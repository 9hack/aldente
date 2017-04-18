#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Quad
{
public:
    Quad(GLfloat width, GLfloat height); // pass me a texture ID
    void draw();
private:
    GLuint VAO, VBO;
};
