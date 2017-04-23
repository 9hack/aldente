#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaders/shader.h"
#include "model/plane.h"

class SceneCamera {
private:
    const glm::vec3 DEFAULT_CAM_POS = glm::vec3(0.0f, 0.0f, 20.0f);
    const glm::vec3 DEFAULT_CAM_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
    const glm::vec3 DEFAULT_CAM_UP = glm::vec3(0.0f, 1.0f, 0.0f);

public:
    glm::vec3 cam_pos;
    glm::vec3 cam_front;
    glm::vec3 cam_up;
    glm::mat4 P;
    glm::mat4 V;
    GLfloat pitch;
    GLfloat yaw = -90.f;

    Plane frustum_planes[6];
    glm::vec3 frustum_corners[8];

    SceneCamera();

    void recalculate();

    void reset();

    void update_frustum_corners(int width, int height, GLfloat);

    void update_frustum_planes();

    void displace_cam(glm::vec3 displacement);

    glm::mat4 frustum_ortho(glm::vec3 light_pos);
};

