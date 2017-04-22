#include "window.h"
#include "events.h"

static const bool FULLSCREEN = false;
auto Window::registry = std::unordered_map<GLFWwindow *, Window *>();

Window::Window(int width, int height, const std::string &name) :
    width(width), height(height) {
    // Create the GLFW window
    GLFWmonitor *monitor = NULL;
    if (FULLSCREEN) {
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        width = mode->width;
        height = mode->height;
    }
    gl_window = glfwCreateWindow(width, height, name.c_str(), monitor, NULL);

    // Check if the window could not be created
    assert(gl_window);
    registry[gl_window] = this;

    // Make the context of the window
    glfwMakeContextCurrent(gl_window);

    // Set swap interval to 1
    glfwSwapInterval(1);

    // Get the width and height of the framebuffer to properly resize the window
    glfwGetFramebufferSize(gl_window, &width, &height);


    //glfwSetInputMode(gl_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // Don't show cursor

    // Set up event dispatchers
    glfwSetFramebufferSizeCallback(gl_window, resize_callback);
    glfwSetKeyCallback(gl_window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        events::WindowKeyData d = {lookup(window), key, scancode, action, mods};
        events::window_key_event(d);
    });
    glfwSetCursorPosCallback(gl_window, [](GLFWwindow *window, double x_pos, double y_pos) {
        events::WindowCursorData d = {lookup(window), x_pos, y_pos};
        events::window_cursor_event(d);
    });
    glfwSetMouseButtonCallback(gl_window, [](GLFWwindow *window, int button, int action, int mods) {
        events::WindowMouseButtonData d = {lookup(window), button, action, mods};
        events::window_mouse_button_event(d);
    });
    glfwSetScrollCallback(gl_window, [](GLFWwindow *window, double x_off, double y_off) {
        events::WindowCursorData d = {lookup(window), x_off, y_off};
        events::window_cursor_event(d);
    });
}

Window::~Window() {
    glfwDestroyWindow(gl_window);
}

Window *Window::lookup(GLFWwindow * target) {
    auto found = registry.find(target);
    return found != registry.end() ? found->second : nullptr;
}

void Window::set_hints() {
    // 4x antialiasing
    glfwWindowHint(GLFW_SAMPLES, 4);
#if defined(__linux) || defined(__APPLE__)
    // Ensure minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

#ifdef __linux
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#else // Apple is dumb
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
#endif

    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void Window::resize_callback(GLFWwindow *window, int w, int h) {
    events::WindowSizeData d = {lookup(window), w, h};
    events::window_resize_event(d);
}

void Window::close() {
    glfwSetWindowShouldClose(gl_window, GL_TRUE);
}

bool Window::should_close() {
    return (bool) glfwWindowShouldClose(gl_window);
}

void Window::swap_buffers() {
    glfwSwapBuffers(gl_window);
}

void Window::clear() {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::pair<int, int> Window::get_size() {
    return {width, height};
};

void Window::update_size() {
    glfwGetFramebufferSize(gl_window, &width, &height);
    resize_callback(gl_window, width, height);
}

std::pair<double, double> Window::get_cursor() {
    double x_pos, y_pos;
    glfwGetCursorPos(gl_window, &x_pos, &y_pos);
    return {x_pos, y_pos};
};
