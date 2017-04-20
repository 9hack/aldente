#include "ui_rectangle.h"

#include "shaders/shader_manager.h"

UIRectangle::UIRectangle(UIElement *parent, UIPosition pos, UIDims dims, bool interactable, glm::vec3 color)
{
	this->parent = parent;
	this->pos = pos;
	this->dims = dims;
	this->interactable = interactable;
	this->color = color;
}

void UIRectangle::draw(Render2D *renderer2D)
{
    if (enabled)
        renderer2D->render_rect(ShaderManager::get_shader_program("text"),
            pos.x, pos.y, dims.width, dims.height, color, 0);
}
