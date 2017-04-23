#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdlib.h>
#include <iostream>

#include "scene/scene.h"
#include "scene/scene_camera.h"

/*
	Main Game Function.
	Game loop and scene setup happens here
*/
class Aldente {
public:
    ~Aldente();

    void start_game_loop();    // Main Game Loop
};
