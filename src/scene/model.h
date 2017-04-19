#pragma once
#include "scene_node.h"
#include "mesh.h"

#include <vector>

#include "scene.h"
#include "shaders/shader.h"

class Model :
	public SceneNode
{
public:
	std::vector<Mesh *> meshes;
	glm::mat4 model_mat;

	Model(Scene *);
	~Model();
	void add_mesh(Mesh *m);
	void combine_meshes();

	void draw();
	void update();
	void pass(Shader *s);
};

