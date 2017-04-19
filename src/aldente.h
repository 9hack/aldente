#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdlib.h>
#include <iostream>

#include "shaders/shader_manager.h"
#include "scene/scene.h"
#include "grid.h"
#include "assetLoader.h"

class Aldente
{
private:
	GLFWwindow *window;

	void handle_movement();
	void setup_scenes();
	void destroy();	
public:
	Aldente();
	~Aldente();

	void go();
	void shadow_pass();
};
