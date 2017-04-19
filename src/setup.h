#pragma once

/*
	Setup for all low-level systems are done here.
	Including OpenGL, glfw, callbacks, and shaders. 
*/
class Setup
{

private:	
	static void setup_callbacks();
	static void setup_opengl();
	static void setup_shaders();

public:
	static void setup_opengl(); // Will handle all OpenGL and GLFW Setup
	
	static void setup_network();
};

