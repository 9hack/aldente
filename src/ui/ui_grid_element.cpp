#include "ui_grid_element.h"

UIGridElement::UIGridElement(UI::ID ui_container, UI::ID element_meat)
{
	this->ui_container = ui_container;
	this->element_meat = element_meat;
}

void UIGridElement::toggle()
{
	selected = !selected;
	UI::ui->set_enabled(ui_container, selected, false);
}

UI::ID UIGridElement::get_id()
{
	return ui_container;
}