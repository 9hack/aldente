#include "ui_image_node.h"

#include "shaders/shader_manager.h"

UIImageNode::UIImageNode(UIElement *parent, UIPosition pos, UIDims dims, bool interactable, glm::vec3 color, GLuint textureID)
{
	this->parent = parent;
	this->pos = pos;
    this->dims = dims;
	this->interactable = interactable;
	this->color = color;
    this->textureID = textureID;
}

void UIImageNode::draw(Render2D *renderer2D)
{
    if (enabled)
        renderer2D->render_rect(ShaderManager::get_shader_program("text"),
            pos.x, pos.y, dims.width, dims.height, color, textureID);
}
