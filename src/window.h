#ifndef _WINDOW_H_
#define _WINDOW_H_

#define GLFW_INCLUDE_GLEXT

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

/*
	Handles GLFW calls for creating and resizing the window.
	Currently assumes there to only be one window. 
*/
class Window
{
public:
	static int width, height; // Width and Height of last updated window

    static GLFWwindow* create_window();
	static void error_callback(int error, const char* description);
	static void resize_callback(GLFWwindow* window, int width, int height);
	
	static void poll_events();
	static int should_close(GLFWwindow* window);
	static void swap_buffers(GLFWwindow* window);
	static void clear_window();
	static void update_size(GLFWwindow* window);
	static void destroy(GLFWwindow *window);
};

#endif
