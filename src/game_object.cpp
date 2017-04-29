#include "game_object.h"

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
    // Test code for playing animation for the boy
    player.set_speed(3.0f);

    if (model->animations.size() > 0) {
        player.play(model, "walk");
        /*
        if (cur_time < 1.5f)
            player.play(cur_time, model->animations["look"], model);
        else
            player.play(cur_time, model->animations["wave"], model);
        */
    }    
}

void GameObject::set_color(glm::vec3 color) {
    for (Mesh* mesh : model->meshes) {
        mesh->material->diffuse = color;
    }
}
