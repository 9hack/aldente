#include "ui_text_node.h"

#include "shaders/shader_manager.h"

UITextNode::UITextNode(UIElement *parent, UIPosition pos, bool interactable, glm::vec3 color, std::string text, float scale)
{
	this->parent = parent;
	this->pos = pos;
	this->interactable = interactable;
	this->color = color;
    this->text = text;
    this->text_scale = scale;
}

void UITextNode::draw(Render2D *renderer2D)
{
    if (enabled)
        renderer2D->render_text(ShaderManager::get_shader_program("text"), text,
            pos.x, pos.y, text_scale, color);
}
