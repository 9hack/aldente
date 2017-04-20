#pragma once

#include "render2d.h"
#include "ui_element.h"

#include <unordered_map>
#include <string>
#include <GL/glew.h>

class UI
{
public:
	typedef unsigned int ID;

    UI(int width, int height);
    void update();
    UI::ID createRectangle(UI::ID parentID, int x_offset, int y_offset, int width, int height, bool interactable, glm::vec3 color);
    UI::ID createTextNode(UI::ID parentID, int x_offset, int y_offset, bool interactable, glm::vec3 color, std::string text, float scale);
    UI::ID createImageNode(UI::ID parentID, int x_offset, int y_offset, int width, int height, bool interactable, glm::vec3 color, GLuint textureID);
	void setEnabled(UI::ID elementID, bool enable);
private:
    // Helper functions
    UIElement* getElementByID(UI::ID id);
    UIElement::UIPosition derivePositionFromParent(UIElement *parent, int x_offset, int y_offset);
    UI::ID finalizeElementAddition(UIElement *new_element, UIElement *parent, int z_depth, bool interactable);

    Render2D *renderer2D;
	std::unordered_map<int, std::vector<UIElement *>> z_buffers;
	std::unordered_map<int, UIElement *> id_lookup;
	std::vector<UIElement *> interactables;
	unsigned int max_z_depth;
	UI::ID next_id = 1;
};
