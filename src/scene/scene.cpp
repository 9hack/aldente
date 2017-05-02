#include "scene.h"

void Scene::draw(Shader *shader) {
    SceneInfo scene_info = { &camera, light_pos };
    for (GameObject *gameObj : objs) {
        gameObj->draw(shader, scene_info);
    }
}

void Scene::draw_skybox() {
    SceneInfo scene_info = { &camera, light_pos };
    ShaderManager::skybox.use();
    ShaderManager::skybox.draw(NULL, scene_info); // NULL because no mesh needed.
}

void Scene::update() {
    for (GameObject *gameObj : objs) {
        gameObj->update();
    }
}

void Scene::addRigid(btRigidBody* toAdd) {
    if (toAdd != NULL) {
        rigids.push_back(toAdd);
        rigidSignal({true, toAdd});
    }
}

void Scene::remove_rigid(btRigidBody* to_remove) {
    rigidSignal({ false, to_remove });
}