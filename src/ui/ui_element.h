#pragma once

#include <vector>
#include <glm/glm.hpp>

class UIElement
{
public:
	struct UIPosition
	{
		int x, y, z;
	};
	struct UIDims
	{
		int width, height;
	};

	UIElement(UIElement *parent, UIPosition pos, UIDims dims, bool interactable, glm::vec3 color);
	void add_child(UIElement *child);
	void remove_child(UIElement *child);
	void enable();
	void disable();

	UIPosition pos;
	UIDims dims;
	bool interactable;
	glm::vec3 color;
	bool enabled;
private:
	UIElement *parent;
	std::vector<UIElement *> children;
};