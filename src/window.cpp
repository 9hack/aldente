#include "window.h"

const bool FULLSCREEN = false;

GLFWwindow* Window::create_window(int width, int height, const char *window_title)
{
    // Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }

    // 4x antialiasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create the GLFW window
	GLFWmonitor* monitor = NULL;
	if (FULLSCREEN)
	{
		monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		width = mode->width;
		height = mode->height;
	}
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, monitor, NULL);

    // Check if the window could not be created
    if (!window)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window
    glfwMakeContextCurrent(window);

    // Set swap interval to 1
    glfwSwapInterval(1);

    // Get the width and height of the framebuffer to properly resize the window
    glfwGetFramebufferSize(window, &width, &height);

	return window;
}
