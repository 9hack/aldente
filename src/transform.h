#pragma once

#include <glm/glm.hpp>

class Transform {
public:
    Transform();    

    void set_world_mat(glm::mat4 mat);

    void set_position(glm::vec3 position);
    void set_position(float x, float y, float z);
    void set_rotation(glm::vec3 angles);
    void set_rotation(float x, float y, float z);
    void set_scale(glm::vec3 scale);
    void set_scale(float x, float y, float z);

    void translate(glm::vec3 position);
    void translate(float x, float y, float z);
    void rotate(glm::vec3 angles);
    void rotate(float x, float y, float z);

    glm::mat4 get_world_mat();
    glm::vec3 get_position();
    glm::vec3 get_rotation();
    glm::vec3 get_scale();

private:  
    glm::mat4 world_mat;
    
    glm::mat4 get_translation_mat();
    glm::mat4 get_rotation_mat();
    glm::mat4 get_scale_mat();
};