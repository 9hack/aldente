#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Geometry {
public:
    bool has_texture;
    GLuint texture;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> tex_coords;
    std::vector<GLuint> indices;

    GLenum draw_type = GL_TRIANGLES;
    GLint wrap_type = GL_REPEAT;
    GLint filter_type = GL_NEAREST_MIPMAP_LINEAR;

    Geometry(GLenum draw = GL_TRIANGLES,
    GLint wrap = GL_REPEAT,
    GLint filter = GL_NEAREST_MIPMAP_LINEAR);

    ~Geometry();

    void populate_buffers();

    void attach_texture(GLuint tex_id);

    GLuint get_texture();

    void draw();

    void bind();

private:
    GLuint VAO, VBO, NBO, TBO, EBO;
};