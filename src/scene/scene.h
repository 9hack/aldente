#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game_objects/game_object.h"
#include "shaders/shader.h"
#include "scene_info.h"

/*
    Contains all the information for a single scene
*/
class Scene {
public:
    SceneInfo info;
    //std::vector<SceneCamera*> cameras;
    std::vector<GameObject*> objs;

    virtual ~Scene() {}

    // Draw using the passed in shader.
    // Default is NULL so that the shader attached to each mesh is used instead.
    void draw(Shader *shader = NULL);
    void draw_skybox();
    SceneCamera &get_cam();

    virtual void update();
    virtual void setup_scene() = 0;
};