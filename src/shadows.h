#pragma once

#include "scene/scene.h"
#include "window.h"

/*
	Handles low level calls needed for running shadows
*/
class Shadows {
private:
    Window &window;

    int screen_width, screen_height;

    bool debug_shadows_on;

public:
    Shadows(Window &window);

    void shadow_pass(Scene *scene);

    void debug_shadows();
};
