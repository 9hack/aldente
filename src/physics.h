#pragma once

#include <glm/glm.hpp>

#include "btBulletDynamicsCommon.h"
#include "game_objects/tile.h"
#include "scene/scene.h"
#include <vector>
#include "events.h"

class Physics {
private:
    Scene *scene;

    //Bullet varaibles
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    std::map<Scene*, btDiscreteDynamicsWorld*> scene_worlds;
public:
    Physics();

    ~Physics();

    btDiscreteDynamicsWorld *dynamicsWorld;
    //std::vector<btRigidBody *> rigidBodies;
    Tile *hover;

    void set_scene(Scene *s);

    void raycast_mouse(double xpos, double ypos, int width, int height);

    void update();

    void raycast(glm::vec3 position, glm::vec3 dir);

    void add_rigid(events::RigidBodyData d);
};

