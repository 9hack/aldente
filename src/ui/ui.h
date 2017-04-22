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

    static UI* ui; // Use this static reference instead of your own call to the constructor

    void setup(int width, int height);
    void update();
    UI::ID create_rectangle(UI::ID parent_id, int x_offset, int y_offset, int width, int height, bool interactable, glm::vec3 color);
    UI::ID create_text_node(UI::ID parent_id, int x_offset, int y_offset, bool interactable, glm::vec3 color, std::string text, float scale);
    UI::ID create_image_node(UI::ID parent_id, int x_offset, int y_offset, int width, int height, bool interactable, glm::vec3 color, GLuint texture_id);
	void set_enabled(UI::ID element_id, bool enable, bool recurse);
private:
    // Private constructor
    UI();

    // Helper functions
    UIElement* get_element_by_id(UI::ID id);
    UIElement::UIPosition derive_position_from_parent(UIElement *parent, int x_offset, int y_offset);
    UI::ID finalize_element_addition(UIElement *new_element, UIElement *parent, int z_depth, bool interactable);

    Render2D *renderer_2d;
	std::unordered_map<int, std::vector<UIElement *>> z_buffers;
	std::unordered_map<int, UIElement *> id_lookup;
	std::vector<UIElement *> interactables;
	unsigned int max_z_depth;
	UI::ID next_id = 1;
};
