#pragma once

#include "scene/scene.h"
#include "window.h"

/*
	Handles low level calls needed for running shadows
*/
class Shadows {
private:
    Window &window;

public:
    Shadows(Window &window) : window(window) {};

    void shadow_pass(Scene *scene);

    void debug_shadows();
};

