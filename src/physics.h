#pragma once

#include <glm/glm.hpp>

#include "btBulletDynamicsCommon.h"
#include "tile.h"
#include "scene/scene.h"
#include <vector>

class Physics {
private:
	Scene *scene;

    //Bullet varaibles
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
	std::map<Scene*, btDiscreteDynamicsWorld*> scene_worlds;

	boost::signals2::connection currentRigidSignal;
public:
	Physics();

    ~Physics();

    btDiscreteDynamicsWorld *dynamicsWorld;
    //std::vector<btRigidBody *> rigidBodies;
    Tile *hover;

    void set_scene(Scene *s);

    void raycast_mouse(double xpos, double ypos, int width, int height);

    void update();

};

