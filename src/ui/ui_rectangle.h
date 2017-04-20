#pragma once

#include "ui_element.h"

class UIRectangle : public UIElement
{
public:
    UIRectangle(UIElement *parent, UIPosition pos, UIDims dims, bool interactable, glm::vec3 color);
    void draw(Render2D *renderer2D) override;
};
