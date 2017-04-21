#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdlib.h>
#include <iostream>

#include "main_scene.h"
#include "scene\scene_camera.h"

/*
	Main Game Function.
	Game loop and scene setup happens here
*/
class Aldente
{
private:	
	// Scenes
	static std::vector<Scene *> scenes;
	static MainScene *main_scene;
	
	static GLFWwindow *window;
	static Scene *scene;
	static SceneCamera* camera;	
	
	static void setup_scenes();
	static void destroy();	

public:	
	static bool shadows_on;
	static bool debug_shadows;

	static void go();	// Main Game Loop

	static std::vector<Scene *> get_scenes(); // Gets list of all scenes in the game
	static Scene *get_scene(); // Gets currently active scene
	static SceneCamera *get_camera(); // Gets currently active camera
	static GLFWwindow *get_window(); // Gets currently active window
};
