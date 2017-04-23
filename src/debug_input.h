#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "scene_manager.h"
#include "window.h"


/*
	Handles GLFW calls for getting Mouse and Keyboard Input (also temporarily holds Camera for debugging)
*/
class DebugInput {
private:
    Window &window;
    bool keys[1024];
    bool lmb_down;
    bool rmb_down;
    bool mouse_moved;
    glm::vec3 last_cursor_pos;
    SceneManager &scene_manager;

public:
    DebugInput(Window &window, SceneManager &scene_manager);
    void handle_movement();
};

