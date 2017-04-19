#pragma once

/*
	Setup for all low-level systems are done here.
	Including OpenGL, glfw, callbacks, and shaders. 
*/
class Setup
{

private:		
public:	
	static void setup_callbacks();
	static void setup_opengl();
	static void setup_shaders();
	static void setup_network();	
	static void setup_bullet();
};

