#include "game_object.h"

GameObject::GameObject() {
	model = new Model();
}

// Attaches a model to the game object
void GameObject::attach_model(Model *m) {
    model = m;
}

// Renders model in scene
void GameObject::draw(SceneCamera &camera) {
    if (model)
        model->draw(camera, transform.get_world_mat());
}

// Updates Game Object Paramters
void GameObject::update() {        
}

// Used for passing seperate shaders, such as for shadows
void GameObject::pass(Shader *s) {
    if (model)
        model->pass(s, transform.get_world_mat());
}

void GameObject::set_color(glm::vec3 color) {
    for (Mesh* mesh : model->meshes) {
        mesh->material->diffuse = color;
    }
}
