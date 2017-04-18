#pragma once
#include "scene_node.h"
#include "mesh.h"

#include <vector>

#include "scene.h"
#include "shaders/shader.h"

class SceneModel :
	public SceneNode
{
public:
	std::vector<Mesh*> meshes;
	glm::mat4 model_mat;

	SceneModel(Scene *);
	SceneModel() {};
	~SceneModel();
	void setScene(Scene* scene);
	void add_mesh(Mesh m);
	void combine_meshes();

	void draw();
	void update();
	void pass(Shader *s);
};

