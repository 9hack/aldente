#pragma once

#include "render2d.h"
#include "ui_element.h"

#include <unordered_map>

class UI
{
public:
	typedef unsigned int ID;

    UI(int width, int height);
    void update();
	UI::ID createUIElement(UI::ID parent, int x_offset, int y_offset, int width, int height, bool interactable, glm::vec3 color);
	void setEnabled(UI::ID elementID, bool enable);
private:
    Render2D *renderer2D;
	std::unordered_map<int, std::vector<UIElement *>> z_buffers;
	std::unordered_map<int, UIElement *> id_lookup;
	std::vector<UIElement *> interactables;
	unsigned int max_z_depth;
	UI::ID next_id = 1;
};
