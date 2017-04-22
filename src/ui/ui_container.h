#pragma once

#include "ui_element.h"

class UIContainer : public UIElement {
public:
    UIContainer() {} // default ctor
    UIContainer(float start_x, float start_y)
        : UIElement(start_x, start_y) {}

    virtual void attach(UIElement *child);
    virtual void detach(UIElement *child);
    void enable() override;
    void disable() override;

    virtual void draw(Render2D &renderer_2d,
        float offset_x, float offset_y) override;
protected:
    std::vector<UIElement *> children;
};
