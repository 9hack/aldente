#include "construct.h"
#include "asset_loader.h"

Construct::Construct(int x, int z) {
    transform.set_position(x, 0.0f, z);
}

Crate::Crate(int x, int z) : Construct(x, z) {
    model = AssetLoader::get_model("chest_good");
    transform.set_scale(0.75f, 0.75f, 0.75f);

    btDefaultMotionState *motionstate = new btDefaultMotionState(btTransform(
        btQuaternion(), btVector3((btScalar)x, 0.5f, (btScalar)z)));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
        0,                  // mass, in kg. 0 -> Static object, will never move.
        motionstate,
        box,  // collision shape of body
        btVector3(0, 0, 0)    // local inertia
    );
    rigidBody = new btRigidBody(rigidBodyCI);

    // Will be used to know which object is picked.
    rigidBody->setUserPointer(this);

    // TODO: Add rigid body to scene.
}

// Activated when a player presses A on it
void Crate::interact_trigger() {
    model = AssetLoader::get_model("veggie");
}