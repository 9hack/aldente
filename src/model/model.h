#pragma once

/*
	A Model is made up of a bunch of meshes to form a 3D renderable object.
*/

#include <vector>

#include "shaders/shader.h"
#include "mesh.h"
#include "scene/scene_camera.h"

class Model {
public:
    std::vector<Mesh *> meshes;
    glm::mat4 model_mat;

    Model();

    void add_mesh(Mesh *m);

    void combine_meshes();

    void draw(SceneCamera* scene);

    void update();

    void pass(Shader *s);
};

