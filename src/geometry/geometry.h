#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Geometry
{
public:
	bool has_texture;
	bool has_normals;
	bool add_texture_noise; //For Water Specifically
	GLuint texture;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> tex_coords;
	std::vector<GLuint> indices;	
	GLenum draw_type = GL_TRIANGLES;
	GLint wrap_type = GL_REPEAT;
	GLint filter_type = GL_NEAREST_MIPMAP_LINEAR;

	Geometry();
	~Geometry();
	void populate_buffers();
	void attach_texture(const char *texture_loc);
	void draw();
	void bind();
private:
	GLuint VAO, VBO, NBO, TBO, EBO;
};