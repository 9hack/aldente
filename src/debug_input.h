#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "scene_manager.h"
#include "physics.h"

/*
	Handles GLFW calls for getting Mouse and Keyboard Input (also temporarily holds Camera for debugging)
*/
class DebugInput {
private:
	bool keys[1024];
    bool lmb_down;
    bool rmb_down;
    bool mouse_moved;
    glm::vec3 last_cursor_pos;
    SceneManager &scene_manager;
	Physics &physics;

public:
    DebugInput(SceneManager &scene_manager, Physics &p);
    void handle_movement();
};

