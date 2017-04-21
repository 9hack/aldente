#pragma once

#include "ui.h"

class UIGridElement
{
public:
	UIGridElement(UI::ID ui_container, UI::ID element_meat);
	void toggle();
	UI::ID get_id();
private:
	UI::ID ui_container;
	UI::ID element_meat;
	bool selected = false;
};
