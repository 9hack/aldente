#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

/*
    The overall color of the model, as well as how it is
    affected by the lighting in the scene.
*/
class Material {
public:
    Material(glm::vec3 diffuse = {1.f,1.f,1.f},
            glm::vec3 ambient = {0.f,0.f,0.f},
            glm::vec3 specular = {0.f,0.f,0.f},
            GLfloat shininess = 50.f, bool shadows = true)
        : ambient(ambient), diffuse(diffuse), specular(specular),
          shininess(shininess), shadows(shadows) {}

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLfloat shininess;
    bool shadows;
};

