#include "ui_element.h"

#include <algorithm>

// Enables rendering of this element and all its descendants.
void UIElement::enable()
{
	enabled = true;
	for (auto it = children.begin(); it != children.end(); ++it)
	{
		(*it)->enable();
	}
}

// Disables rendering of this element and all its descendants.
void UIElement::disable()
{
	enabled = false;
	for (auto it = children.begin(); it != children.end(); ++it)
	{
		(*it)->disable();
	}
}

void UIElement::add_child(UIElement *child)
{
	children.push_back(child);
}

void UIElement::remove_child(UIElement *child)
{
	auto it = std::find(children.begin(), children.end(), child);
	if (it != children.end()) children.erase(it);
}
