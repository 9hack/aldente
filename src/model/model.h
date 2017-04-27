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

    void add_mesh(Mesh *m);

    void combine_meshes();

    void draw(SceneCamera &scene, glm::mat4 to_world);

    void update();

    void pass(Shader *s, glm::mat4 to_world);
};

