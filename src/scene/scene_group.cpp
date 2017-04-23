#include "scene_group.h"

SceneGroup::SceneGroup() {}

SceneGroup::SceneGroup(Scene *scene) {
    this->scene = scene;
}

SceneGroup::~SceneGroup() {
    for (SceneNode* node : children)
        delete (node);
}

void SceneGroup::add_child(SceneNode *node) {
    children.push_back(node);
}

void SceneGroup::remove_all() {
    for (SceneNode* node : children)
        delete (node);
    children.clear();
}

void SceneGroup::draw() {
    for (SceneNode* node : children)
        (node)->draw();
}

void SceneGroup::update() {
    for (auto it = children.begin(); it != children.end(); ++it)
        (*it)->update();
}

void SceneGroup::pass(Shader *s) {
    for (auto it = children.begin(); it != children.end(); ++it)
        (*it)->pass(s);
}