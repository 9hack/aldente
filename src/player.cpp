#include "player.h"

#include "asset_loader.h"
#include "events.h"
Player::Player(Scene *scene) : GameObject(scene) {
    to_moveX = 0;
    to_moveZ = 0;
    speed = 10.0f;

    btDefaultMotionState *motionstate = new btDefaultMotionState(btTransform(
        btQuaternion(), btVector3(2.0f, 0.0f, 2.0f)));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
        1,                  
        motionstate,
        box,  // collision shape of body
        btVector3(0, 0, 0)    // local inertia
    );
    rigidBody = new btRigidBody(rigidBodyCI);

    // Will be used to know which object is picked.
    rigidBody->setUserPointer(this);
    
    // Lock y-axis
    rigidBody->setLinearFactor(btVector3(1, 0, 1));

    scene->addRigid(rigidBody);

    setup_listeners();

    transform.set_position(2.0f, 0.0f, 2.0f);
}

void Player::setup_listeners() {
    events::dungeon::player_move_event.connect([&](Direction dir) {

        // Sets the to_move as 1 for now but should parse
        // the degree of the joystick as well i.e a slight
        // push should move the player less than a full push.
        // Also should account for more than one axis.
        switch (dir) {
        case UP:
            to_moveZ = -1;
            to_moveX = 0;
            break;
        case DOWN:
            to_moveZ = 1;
            to_moveX = 0;
            break;
        case LEFT:
            to_moveX = -1;
            to_moveZ = 0;
            break;
        case RIGHT:
            to_moveX = 1;
            to_moveZ = 0;
            break;
        default:
            break;
        }
    });
}

// Just calls do_movement for now, can have more
// functionality later.
void Player::update() {
    do_movement();

    btTransform t;

    // Get the transform from Bullet and into 't'
    rigidBody->getMotionState()->getWorldTransform(t);
    fprintf(stderr, "%f,%f,%f\n", t.getOrigin().getX(), t.getOrigin().getY(), t.getOrigin().getZ());
    transform.set_position(glm::vec3((float)t.getOrigin().getX(), (float)t.getOrigin().getY(),
        (float)t.getOrigin().getZ()));
}

void Player::do_movement() {
    // Should account for deltatime so movement is
    // framerate independent.
    //transform.translate(to_moveX * speed, 0, to_moveZ * speed);
    rigidBody->setActivationState(true);
    rigidBody->setLinearVelocity(btVector3(to_moveX * speed, 0, to_moveZ * speed));
}