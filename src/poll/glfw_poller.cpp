#include "glfw_poller.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void GlfwPoller::poll() {
    glfwPollEvents();
}
