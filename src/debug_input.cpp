#include "debug_input.h"

#include <tuple>
#include "aldente.h"
#include "physics.h"
#include "events.h"
#include "util/config.h"

// TODO : This class won't be used in the final game anyways.
static const GLfloat BASE_CAM_SPEED = 0.1f;
static const GLfloat EDGE_PAN_THRESH = 5.f;
static const GLfloat EDGE_PAN_SPEED = 0.5f;

// FPS Tracking Variables
static GLuint frame = 0;
static double prev_ticks = glfwGetTime();
static double move_prev_ticks = prev_ticks;

// TODO: this is disgusting
DebugInput::DebugInput(SceneManager &scene_manager) : scene_manager(scene_manager) {
    // Set up callbacks
    events::window_resize_event.connect([&](events::WindowSizeData d) {
        // Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
        glViewport(0, 0, d.width, d.height);

        // TODO: this is dirty
        float far_plane, fov;
        Config::config->get_value(Config::str_far_plane, far_plane);
        Config::config->get_value(Config::str_fov, fov);

        if (d.height > 0) {
            for (Scene *s : scene_manager.get_scenes())
                s->camera->P = glm::perspective(fov, (float) d.width / (float) d.height, 0.1f, far_plane);
        }
    });

    events::window_key_event.connect([&](events::WindowKeyData d) {
        // Check for a key press
        if (d.action == GLFW_PRESS) {
            keys[d.key] = true;
            switch (d.key) {
                // Check if escape was pressed
                case GLFW_KEY_ESCAPE:
                    // Close the window. This causes the program to also terminate.
                    d.window->close();
                    break;
                case GLFW_KEY_Q:
                    Aldente::debug_shadows = !Aldente::debug_shadows;
                    break;
                case GLFW_KEY_X:
                    Aldente::shadows_on = !Aldente::shadows_on;
                    break;
                default:
                    break;
            }
        } else if (d.action == GLFW_RELEASE) {
            keys[d.key] = false;
        }
    });

    events::window_cursor_event.connect([&](events::WindowCursorData d) {
        int width, height;
        std::tie(width, height) = d.window->get_size();
        glm::vec3 current_cursor_pos(d.x_pos, d.y_pos, 0);

        Scene *scene = scene_manager.get_scene();
        SceneCamera *camera = scene_manager.get_camera();

        // First movement detected.
        if (!mouse_moved) {
            mouse_moved = true;
            last_cursor_pos = current_cursor_pos;
            return;
        }

        glm::vec3 cursor_delta = current_cursor_pos - last_cursor_pos;
        if (lmb_down && keys[GLFW_KEY_LEFT_CONTROL]) {
            int dir = cursor_delta.x > 0 ? 1 : -1;
            float rot_angle = dir * glm::length(cursor_delta) * 0.001f;
            scene->light_pos = glm::vec3(
                    glm::rotate(glm::mat4(1.0f), rot_angle, glm::vec3(0.f, 0.f, 1.f)) * glm::vec4(scene->light_pos, 1.0f));
        } else if (!keys[GLFW_KEY_LEFT_CONTROL]) {
            // Look around.
            GLfloat xoffset = cursor_delta.x;
            GLfloat yoffset = cursor_delta.y;
            GLfloat sensitivity = 0.5;
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            if (current_cursor_pos.x > width - EDGE_PAN_THRESH)
                xoffset = EDGE_PAN_SPEED;
            else if (current_cursor_pos.x < EDGE_PAN_THRESH)
                xoffset = -EDGE_PAN_SPEED;

            camera->yaw += xoffset;
            camera->pitch += yoffset;

            if (camera->pitch > 89.0f)
                camera->pitch = 89.0f;
            if (camera->pitch < -89.0f)
                camera->pitch = -89.0f;

            glm::vec3 front;
            front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
            front.y = -sin(glm::radians(camera->pitch));
            front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
            camera->cam_front = glm::normalize(front);
            camera->recalculate();
        }

        Physics::physics->raycast_mouse(d.x_pos, d.y_pos, width, height);

        last_cursor_pos = current_cursor_pos;
		std::fill(std::begin(keys), std::end(keys), false);
    });

    events::window_mouse_button_event.connect([&](events::WindowMouseButtonData d) {
        double x_pos, y_pos;
        std::tie(x_pos, y_pos) = d.window->get_cursor();

        switch (d.button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                if (d.action == GLFW_PRESS) {
                    lmb_down = true;
                    last_cursor_pos = glm::vec3(x_pos, y_pos, 0.f);
                } else if (d.action == GLFW_RELEASE) {
                    lmb_down = false;
                }
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                if (d.action == GLFW_PRESS) {
                    rmb_down = true;
                    last_cursor_pos = glm::vec3(x_pos, y_pos, 0.f);
                } else if (d.action == GLFW_RELEASE) {
                    rmb_down = false;
                }
                break;
            default:
                break;
        }
    });

    events::window_scroll_event.connect([&](events::WindowScrollData d) {
        SceneCamera *camera = scene_manager.get_camera();
        glm::vec3 trans_vec = (float) d.y_off * glm::normalize(camera->cam_front);
        // Only y is relevant here, -1 is down, +1 is up
        camera->cam_pos = glm::vec3(glm::translate(glm::mat4(1.0f), trans_vec) * glm::vec4(camera->cam_pos, 1.0f));
        camera->recalculate();
    });
};


void DebugInput::handle_movement() {
    // FPS Control / Tracking
    frame++;
    double curr_time = glfwGetTime();
    if (curr_time - prev_ticks > 1.f) {
        /* std::cerr << "FPS: " << frame << std::endl; */
        frame = 0;
        prev_ticks = curr_time;
        return;
    }
    if (curr_time - move_prev_ticks > 1.f / 60.f)
        move_prev_ticks = curr_time;

    SceneCamera *camera = scene_manager.get_camera();

    GLfloat cam_step = keys[GLFW_KEY_LEFT_SHIFT] ? 3 * BASE_CAM_SPEED : BASE_CAM_SPEED;
    glm::vec3 displacement(0.f);
    if (keys[GLFW_KEY_W])
        displacement += cam_step * camera->cam_front;
    if (keys[GLFW_KEY_S])
        displacement -= cam_step * camera->cam_front;
    if (keys[GLFW_KEY_A])
        displacement -= glm::normalize(glm::cross(camera->cam_front, camera->cam_up)) * cam_step;
    if (keys[GLFW_KEY_D])
        displacement += glm::normalize(glm::cross(camera->cam_front, camera->cam_up)) * cam_step;
	if (keys[GLFW_KEY_SPACE])
        displacement += cam_step * camera->cam_up;

    camera->cam_pos += displacement;
    camera->recalculate();
}
