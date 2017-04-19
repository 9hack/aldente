#ifndef _WINDOW_H_
#define _WINDOW_H_

#define GLFW_INCLUDE_GLEXT

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

/*
	Handles GLFW calls for creating and resizing the window.
*/
class Window
{
public:
    static GLFWwindow* create_window(int width, int height, const char *window_title);
	static void error_callback(int error, const char* description);
	static void resize_callback(GLFWwindow* window, int width, int height);
};

#endif
