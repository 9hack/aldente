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
    std::vector<btRigidBody*> rigids;
    boost::signals2::signal<void(std::pair<bool,btRigidBody*>)> rigidSignal;

    virtual ~Scene() {}

    // Draw using the passed in shader.
    // Default is NULL so that the shader attached to each mesh is used instead.
    void draw(Shader *shader = NULL);
    void draw_skybox();

    virtual void update();

    void addRigid(btRigidBody* toAdd);
};