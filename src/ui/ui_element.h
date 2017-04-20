#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "render2d.h"

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

	void add_child(UIElement *child);
	void remove_child(UIElement *child);
	void enable();
	void disable();

    virtual void draw(Render2D *renderer2D)=0;

	UIPosition pos;
	UIDims dims;
	bool interactable;
	glm::vec3 color;
    std::string text;
    float text_scale;
	bool enabled = true;
    GLfloat textureID;
	UIElement *parent;
private:
	std::vector<UIElement *> children;
};
