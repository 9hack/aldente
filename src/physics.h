#pragma once

#include <glm/glm.hpp>

#include "btBulletDynamicsCommon.h"
#include "tile.h"
#include <vector>

class Physics
{
private:
	//Bullet varaibles
	static btBroadphaseInterface* broadphase;
	static btDefaultCollisionConfiguration* collisionConfiguration;
	static btCollisionDispatcher* dispatcher;
	static btSequentialImpulseConstraintSolver* solver;	
public:

	static btDiscreteDynamicsWorld* dynamicsWorld;
	static std::vector<btRigidBody*> rigidBodies;
	static Tile *hover;
	
	static void setup_bullet();
	static void raycast_mouse(double xpos, double ypos, int width, int height);
	static void update();
};

