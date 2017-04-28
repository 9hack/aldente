#include "game_object.h"
#include <GLFW/glfw3.h>

double last_time = 0;
double cur_time = 0;

GameObject::GameObject() {
	model = new Model();
}

// Attaches a model to the game object
void GameObject::attach_model(Model *m) {
    model = m;
}

// Renders model in scene
void GameObject::draw(Shader *shader, SceneInfo &scene_info) {
    if (model) model->draw(shader, scene_info, transform.get_world_mat());
}

// Updates Game Object Paramters
void GameObject::update() {
    cur_time += (glfwGetTime() - last_time) * 0.8f;

    if (cur_time > 3)
        cur_time = 0;

    if (model->animations.size() > 0) {
        player.play(cur_time, model->animations[0], model);
    }

    last_time = glfwGetTime();
}

void GameObject::set_color(glm::vec3 color) {
    for (Mesh* mesh : model->meshes) {
        mesh->material->diffuse = color;
    }
}
