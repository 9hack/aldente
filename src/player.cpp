#include "player.h"

#include "asset_loader.h"
#include "events.h"
Player::Player(Scene *scene) : GameObject(scene) {
    to_moveX = 0;
    to_moveZ = 0;
    speed = 10.0f;
    direction = glm::vec3(0.0f);

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
    rigidBody->setLinearFactor(btVector3(1, 0.0f, 1));

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

    events::dungeon::player_interact_event.connect([&]() {
        interact();
    });

    events::dungeon::player_raycast_response_event.connect([&](GameObject *bt_hit) {
        // Second part of the interact function.
        if (dynamic_cast<Construct*>(bt_hit)) {
            dynamic_cast<Construct*>(bt_hit)->interact_trigger();
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
    btVector3 to_set = t.getOrigin();

    transform.set_position(glm::vec3((float)to_set.getX(), (float)to_set.getY(),
        (float)to_set.getZ()));
}

void Player::do_movement() {
    // Should account for deltatime so movement is
    // framerate independent? Unsure how Bullet handles framerate.
    rigidBody->setActivationState(true);
    rigidBody->setLinearVelocity(btVector3(to_moveX * speed, 0, to_moveZ * speed));
    transform.look_at(glm::vec3(to_moveX * speed, 0, to_moveZ * speed));
    direction = glm::vec3(to_moveX * speed, 0, to_moveZ * speed);
}

void Player::interact() {
    // Asks physics for a raycast to check if the player
    // is facing a construct.
    if(direction.x != 0.0f || direction.z != 0.0f)
        events::dungeon::player_request_raycast_event(transform.get_position(), direction);
}