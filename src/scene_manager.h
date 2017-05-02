#pragma once

#include "scene/scene.h"
#include "scene/scene_camera.h"

class SceneManager {
private:
    std::vector<Scene *> scenes;
    Scene *current_scene;
    SceneCamera *camera;

public:
    SceneManager();
    std::vector<Scene *> get_scenes(); // Gets list of all scenes in the game
    Scene *get_current_scene(); // Gets currently active scene
    SceneCamera *get_camera(); // Gets currently active camera

    void add_scene(Scene* scene);
    void set_current_scene(Scene* scene);
};
