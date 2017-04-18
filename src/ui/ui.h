#pragma once

#include "render2d.h"

class UI
{
public:
    UI(int width, int height);
    void update();
private:
    Render2D *renderer2D;
};
