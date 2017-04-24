#pragma once

#include "shadows.h"
#include "window.h"
#include "scene_manager.h"

class Render {
private:
    Shadows shadows;

    Window &window;

    SceneManager &scene_manager;

public:
    Render(Window &window, SceneManager &scene_manager);

    void update();

};
