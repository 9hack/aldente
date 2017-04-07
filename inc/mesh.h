#pragma once
#include <GL/glew.h>
#include "geometry.h"
#include "material.h"
#include "shader.h"
#include <glm/glm.hpp>

struct Mesh
{
	Geometry *geometry;
	Material material;
	Shader *shader;
	glm::mat4 to_world;
	bool no_culling;
};
