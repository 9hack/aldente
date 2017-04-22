#pragma once

#define GLFW_INCLUDE_GLEXT

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <unordered_map>

/*
	Handles GLFW calls for creating and resizing the window.
*/
class Window {
private:
    GLFWwindow *gl_window;
    int width, height;

    static std::unordered_map<GLFWwindow *, Window *> registry;
    static Window *lookup(GLFWwindow * target);

    // We explicitly define this because we need to call this in update_size().
    static void resize_callback(GLFWwindow *window, int w, int h);

public:
    Window(int width, int height, const std::string &name);
    ~Window();

    static void set_hints();
    void clear();
    void close();
    bool should_close();
    void swap_buffers();
    std::pair<int, int> get_size();
    void update_size();
    std::pair<double, double> get_cursor();
};