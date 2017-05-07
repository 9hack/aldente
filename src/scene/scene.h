#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "btBulletDynamicsCommon.h"
#include "game_objects/game_object.h"
#include "shaders/shader.h"
#include "boost/signals2.hpp"

/*
    Contains all the information for a single scene
*/
class Scene {
public:
    SceneCamera camera;
    //std::vector<SceneCamera*> cameras;
    std::vector<GameObject*> objs;
    glm::vec3 light_pos;

    virtual ~Scene() {}

    // Draw using the passed in shader.
    // Default is NULL so that the shader attached to each mesh is used instead.
    void draw(Shader *shader = NULL);
    void draw_skybox();

    virtual void update();

    // Sets up scene but not its models
    virtual void setup_scene() = 0;

    // Sets up scene's models (please call after setup_scene). Do not call on server
    virtual void graphical_setup() = 0;
};