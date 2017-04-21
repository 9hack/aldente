#include "ui_element.h"

#include <algorithm>

// Enables rendering of this element and all its descendants, if recurse is true.
void UIElement::enable(bool recurse)
{
	enabled = true;
	if (recurse)
	{
		for (auto it = children.begin(); it != children.end(); ++it)
		{
			(*it)->enable(recurse);
		}
	}
}

// Disables rendering of this element and all its descendants, if recurse is true.
void UIElement::disable(bool recurse)
{
	enabled = false;
	if (recurse)
	{
		for (auto it = children.begin(); it != children.end(); ++it)
		{
			(*it)->disable(recurse);
		}
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
