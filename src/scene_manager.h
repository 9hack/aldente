#pragma once

#include "scene/main_scene.h"
#include "scene/scene_camera.h"

class SceneManager {
private:
    std::vector<Scene *> scenes;
    MainScene *main_scene;
    Scene *scene;
    SceneCamera *camera;

public:
    SceneManager();

    std::vector<Scene *> get_scenes(); // Gets list of all scenes in the game
    Scene *get_scene(); // Gets currently active scene
    SceneCamera *get_camera(); // Gets currently active camera
};
