#pragma once

#include "shadows.h"
#include "scene_manager.h"

class Render {
private:
    Shadows shadows;

    SceneManager &scene_manager;

public:
    Render(SceneManager &scene_manager);

    void update();

};
