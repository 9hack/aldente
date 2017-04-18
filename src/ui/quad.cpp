#include "quad.h"

Quad::Quad(GLfloat width, GLfloat height)
{
    GLfloat quadVertices[] = {
        // Positions                                  // Texture Coords
        -1.0f * width / 2,  1.0f * height / 2, 0.0f,  0.0f, 1.0f,
        -1.0f * width / 2, -1.0f * height / 2, 0.0f,  0.0f, 0.0f,
         1.0f * width / 2,  1.0f * height / 2, 0.0f,  1.0f, 1.0f,
         1.0f * width / 2, -1.0f * height / 2, 0.0f,  1.0f, 0.0f,
    };
    // Setup plane VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
}

void Quad::draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
