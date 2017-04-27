#pragma once

/*
    A Model is made up of a bunch of meshes to form a 3D renderable object.
*/

#include <vector>

#include "shaders/shader_manager.h"
#include "shaders/shader.h"
#include "mesh.h"
#include "scene/scene_info.h"

class Model {
private:
    Shader *model_shader;

public:
    std::vector<Mesh *> meshes;
<<<<<<< HEAD
=======
    std::map<std::string, GLuint> bone_mapping; // Maps bone name to index
    std::vector<glm::mat4> bones;
    glm::mat4 model_mat;
>>>>>>> Got models working, loading bones

    Model(Shader *shader = &ShaderManager::basic) : model_shader(shader) {}

    void add_mesh(Mesh *m);

    void draw(Shader *shader, SceneInfo &scene_info, glm::mat4 to_world);
};

