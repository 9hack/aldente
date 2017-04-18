#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shaders/shader.h"

class Scene;

class SceneNode
{
protected:
	Scene *scene;
public:
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void pass(Shader *s) = 0;
};

