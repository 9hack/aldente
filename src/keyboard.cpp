#include "keyboard.h"

bool Keyboard::lmb_down = false;
bool Keyboard::rmb_down = false;


void Keyboard::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		keys[key] = true;
		switch (key)
		{
			// Check if escape was pressed
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_Q:
			debug_shadows = !debug_shadows;
			break;
		case GLFW_KEY_X:
			shadows_on = !shadows_on;
			break;
		case GLFW_KEY_G:
			god_mode = !god_mode;
			break;
		default:
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}
}

void Keyboard::cursor_position_callback(GLFWwindow* window, double x_pos, double y_pos)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glm::vec3 current_cursor_pos(x_pos, y_pos, 0);

	// First movement detected.
	if (!mouse_moved)
	{
		mouse_moved = true;
		last_cursor_pos = current_cursor_pos;
		return;
	}

	glm::vec3 cursor_delta = current_cursor_pos - last_cursor_pos;
	if (lmb_down && keys[GLFW_KEY_LEFT_CONTROL])
	{
		int dir = cursor_delta.x > 0 ? 1 : -1;
		float rot_angle = dir * glm::length(cursor_delta) * 0.001f;
		scene->light_pos = glm::vec3(glm::rotate(glm::mat4(1.0f), rot_angle, glm::vec3(0.f, 0.f, 1.f)) * glm::vec4(scene->light_pos, 1.0f));

		/*
		float angle;
		// Horizontal rotation
		angle = (float)(cursor_delta.x) / 100.f;
		camera->cam_pos = glm::vec3(glm::rotate(glm::mat4(1.f), angle, glm::vec3(0.f, 1.f, 0.f)) * glm::vec4(camera->cam_pos, 1.f));
		camera->cam_up = glm::vec3(glm::rotate(glm::mat4(1.f), angle, glm::vec3(0.f, 1.f, 0.f)) * glm::vec4(camera->cam_up, 1.f));

		// Vertical rotation
		angle = (float)(-cursor_delta.y) / 100.f;
		glm::vec3 axis = glm::cross((camera->cam_pos - (camera->cam_pos + camera->cam_front)), camera->cam_up);
		camera->cam_pos = glm::vec3(glm::rotate(glm::mat4(1.f), angle, axis) * glm::vec4(camera->cam_pos, 1.f));
		camera->cam_up = glm::vec3(glm::rotate(glm::mat4(1.f), angle, axis) * glm::vec4(camera->cam_up, 1.f));
		camera->cam_front = glm::normalize(-camera->cam_pos);

		camera->recalculate();
		*/
	}
	else if (!keys[GLFW_KEY_LEFT_CONTROL])
	{
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

	last_cursor_pos = current_cursor_pos;
}

void Keyboard::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double x_pos, y_pos;
	glfwGetCursorPos(window, &x_pos, &y_pos);

	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		if (action == GLFW_PRESS)
		{
			lmb_down = true;
			last_cursor_pos = glm::vec3(x_pos, y_pos, 0.f);
		}
		else if (action == GLFW_RELEASE)
		{
			lmb_down = false;
		}
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		if (action == GLFW_PRESS)
		{
			rmb_down = true;
			last_cursor_pos = glm::vec3(x_pos, y_pos, 0.f);
		}
		else if (action == GLFW_RELEASE)
		{
			rmb_down = false;
		}
		break;
	default:
		break;
	}
}

void Keyboard::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	glm::vec3 trans_vec = (float)yoffset * glm::normalize(camera->cam_front);
	// Only y is relevant here, -1 is down, +1 is up
	camera->cam_pos = glm::vec3(glm::translate(glm::mat4(1.0f), trans_vec) * glm::vec4(camera->cam_pos, 1.0f));
	camera->recalculate();
}