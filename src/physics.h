#pragma once

#include <glm/glm.hpp>

#include "btBulletDynamicsCommon.h"
#include "tile.h"
#include "scene/scene.h"
#include <vector>

class Physics {
private:
    Physics();

    // TODO: truly the unsafest of things
    static Scene *scene;

    //Bullet varaibles
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
public:
    static Physics *physics; // Singleton

    ~Physics();

    btDiscreteDynamicsWorld *dynamicsWorld;
    std::vector<btRigidBody *> rigidBodies;
    Tile *hover;

    static void set_scene(Scene *s);

    void setup_bullet();

    void raycast_mouse(double xpos, double ypos, int width, int height);

    void update();
};

