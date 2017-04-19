#pragma once

#include "scene\scene.h"

/*
	Handles low level calls needed for running shadows
*/
class Shadows
{
public:
	static void shadow_pass(Scene *scene);
	static void debug_shadows(int width, int height);
};

