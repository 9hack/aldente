#pragma once

#include "render2d.h"
#include "ui_element.h"

#include <unordered_map>

class UI
{
public:
    UI(int width, int height);
    void update();
private:
    Render2D *renderer2D;
	std::unordered_map<int, std::vector<UIElement>> zbuffers;
	int zdepth;
};
