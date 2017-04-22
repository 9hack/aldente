#include "scene_manager.h"

SceneManager::SceneManager() {
    // Initial scene and cam.
    main_scene = new MainScene();
    main_scene->setup();
    scene = main_scene;
    scenes.push_back(scene);
    camera = scene->camera;
}

std::vector<Scene *> SceneManager::get_scenes() {
    return scenes;
}

Scene *SceneManager::get_scene() {
    return scene;
}

SceneCamera *SceneManager::get_camera() {
    return camera;
}
