#pragma once

#define GLFW_INCLUDE_GLEXT

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <unordered_map>
#include <mutex>

/*
	High-level GLFWwindow abstraction which dispatches lambda-friendly events.
*/
class Window {
private:
    GLFWwindow *gl_window;
    int width, height;

    // One-time init
    static std::mutex init_lock;
    static bool initted;
    static void init();

    // A static registry of windows is required to statically obtain the Window pointer corresponding to the GLFWwindow
    // we get from a GLFW callback.
    static std::unordered_map<GLFWwindow *, Window *> registry;
    static Window *lookup(GLFWwindow * target);

    // We explicitly define this because we need to call this in update_size().
    static void resize_callback(GLFWwindow *window, int w, int h);

public:
    Window(const std::string &name, bool show_cursor, int width, int height,
           GLFWmonitor *monitor = nullptr, bool fullscreen = false);
    ~Window();

    void close();
    int should_close() const;
    void swap_buffers();
    void clear();
    std::pair<int, int> get_size() const;
    std::pair<double, double> get_cursor() const;
    void update_size();
};