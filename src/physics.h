#pragma once

#include <glm/glm.hpp>

#include "btBulletDynamicsCommon.h"
#include <vector>

class Physics
{
private:
	//Bullet varaibles
	static btBroadphaseInterface* broadphase;
	static btDefaultCollisionConfiguration* collisionConfiguration;
	static btCollisionDispatcher* dispatcher;
	static btSequentialImpulseConstraintSolver* solver;
	static btDiscreteDynamicsWorld* dynamicsWorld;
	static std::vector<btRigidBody*> rigidBodies;

public:
	
	static void setup_bullet();
	static void raycast_mouse(int xpos, int ypos, int width, int height);
	static void update();
};

