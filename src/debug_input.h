#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "scene_manager.h"
#include "physics.h"
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
    glm::vec3 last_cursor_pos;
    SceneManager &scene_manager;
    Physics &physics;

    bool debug_input_on = false;

    // Moving camera using keyboard and mouse
    void debug_movement_keys();
    void debug_movement_mouse(events::WindowCursorData d);

public:
    DebugInput(Window &window, SceneManager &scene_manager, Physics &p);
};

