#pragma once

#include <vector>

#include "render2d.h"

class UIElement {
public:
    virtual void draw(Render2D &renderer_2d,
                      float offset_x, float offset_y) = 0;
	virtual void enable() { enabled = true; }
	virtual void disable() { enabled = false; }
protected:
    bool enabled = true;    // by default, UI elements are shown.
    float start_x, start_y; // starting coordinates (in UI units)
};
