#include "scene.h"

void Scene::draw(Shader *shader) {
    for (GameObject *gameObj : objs) {
        gameObj->draw(shader, info);
    }
}

void Scene::draw_skybox() {
    ShaderManager::skybox.use();
    ShaderManager::skybox.draw(NULL, info); // NULL because no mesh needed.
}

SceneCamera &Scene::get_cam() {
    return info.camera;
}

void Scene::update() {
    for (GameObject *gameObj : objs) {
        gameObj->update();
    }
}