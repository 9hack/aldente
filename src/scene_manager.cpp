#include "scene_manager.h"

std::vector<Scene *> SceneManager::get_scenes() {
    return scenes;
}

Scene *SceneManager::get_current_scene() {
    return current_scene;
}

SceneCamera *SceneManager::get_camera() {
    return camera;
}

void SceneManager::add_scene(Scene* scene) {
	//Make sure you're not adding a duplicate scene
	assert(std::find(scenes.begin(), scenes.end(), scene) == scenes.end());
	scenes.push_back(scene);
}

void SceneManager::set_current_scene(Scene* scene) {
	//Add the scene if not currently added
	if (std::find(scenes.begin(), scenes.end(), scene) == scenes.end()) {
		add_scene(scene);
	}
	current_scene = scene;
	camera = scene->camera;
}