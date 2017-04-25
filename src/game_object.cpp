#include "game_object.h"

GameObject::GameObject() {
	model = new Model();
	to_world = glm::mat4(1.0f);
}

// Attaches a model to the game object
void GameObject::attach_model(Model *m) {
    model = m;
}

// Renders model in scene
void GameObject::draw(SceneCamera &camera) {
    if (model)
        model->draw(camera, to_world);
}

// Updates Game Object Paramters
void GameObject::update() {
}

// Used for passing seperate shaders, such as for shadows
void GameObject::pass(Shader *s) {
    if (model)
        model->pass(s, to_world);
}
/*
glm::vec3 getPosition() {
	//to
	return NULL;
}

glm::vec3 setPosition() {
	//toWorld->
	return NULL;
}*/
