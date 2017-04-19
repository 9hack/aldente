#include "ui.h"

#include "shaders/shader_manager.h"

UI::UI(int width, int height)
{
    renderer2D = new Render2D();
    renderer2D->setup_text(width, height);

	// Setup some test UI.
	UI::ID parent_id = createUIElement(0, 100, 100, 200, 400, false, glm::vec3(226.f, 122.f, 61.f) / 255.f);
	UI::ID child_id = createUIElement(parent_id, 50, 50, 100, 300, false, glm::vec3(1.f, 1.f, 1.f));

	setEnabled(parent_id, true);
}

void UI::update()
{
    renderer2D->render_text(ShaderManager::get_shader_program("text"), "Hello world!", 25.f, 25.f, 1.f, glm::vec3(0.5f, 0.8f, 0.2f));
    //renderer2D->render_rectP(ShaderManager::get_shader_program("text"), 0.1f, 0.1f, 0.5f, 0.5f, glm::vec3(226.f, 122.f, 61.f) / 255.f, 0);

	// Loop through zbuffers.
	for (unsigned int i = 0; i <= max_z_depth; ++i)
	{
		// Check if any elements at this depth.
		auto it = z_buffers.find(i);
		if (it == z_buffers.end()) continue;

		auto list = it->second;
		for (auto list_it = list.begin(); list_it != list.end(); ++list_it)
		{
			UIElement *element = *list_it;
			if (element->enabled)
				renderer2D->render_rect(ShaderManager::get_shader_program("text"),
					element->pos.x, element->pos.y,
					element->dims.width, element->dims.height,
					element->color, 0);
		}
	}
}

void UI::setEnabled(UI::ID elementID, bool enable)
{
	auto it = id_lookup.find(elementID);
	if (it != id_lookup.end())
	{
		if (enable)
			it->second->enable();
		else
			it->second->disable();
	}
}

// Create a UIElement at position relative to parent, as specified by parent ID. 
// No parent if ID is 0. Returns ID of the newly created element.
UI::ID UI::createUIElement(UI::ID parentID, int x_offset, int y_offset, int width, int height, bool interactable, glm::vec3 color)
{
	// Get reference to parent element of the newly created guy, if any.
	int x_pos = x_offset;
	int y_pos = y_offset;
	int z_depth = 0;
	UIElement *parent;
	auto id_it = id_lookup.find(parentID);
	if (id_it == id_lookup.end())
	{
		parent = 0;
	}
	else
	{
		// Calculate x and y relative to parent. z depth is parent's + 1
		parent = id_it->second;
		x_pos += parent->pos.x;
		y_pos += parent->pos.y;
		z_depth = 1 + parent->pos.z;
	}

	// Create the new element!
	UIElement *new_element = new UIElement(parent, { x_pos, y_pos, z_depth },
	                                       { width, height }, interactable, color);

	// Adjust links
	if (parent != 0)
		parent->add_child(new_element);

	// Add to lookup tables as necessary
	auto zbuf_it = z_buffers.find(z_depth);
	if (zbuf_it == z_buffers.end())
		z_buffers[z_depth] = std::vector<UIElement *>();
	z_buffers[z_depth].push_back(new_element);

	id_lookup[next_id] = new_element;

	if (interactable)
		interactables.push_back(new_element);

	// Adjust max z depth as necessary
	max_z_depth = z_depth > max_z_depth ? z_depth : max_z_depth;

	// Return ID of this element, and increment.
	return next_id++;
}
