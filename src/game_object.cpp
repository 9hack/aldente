#include "game_object.h"

GameObject::GameObject() {
}

// Attaches a model to the game object
void GameObject::attach_model(Model *m) {
    model = m;
}

// Renders model in scene
void GameObject::draw(SceneCamera* camera) {
    if (model)
        model->draw(camera);
}

// Updates Game Object Paramters
void GameObject::update() {
}

// Used for passing seperate shaders, such as for shadows
void GameObject::pass(Shader *s) {
    if (model)
        model->pass(s);
}
