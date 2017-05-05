#include "player.h"

#include "asset_loader.h"
#include "events.h"

Player::Player() : GameObject() {
    to_moveX = 0;
    to_moveZ = 0;
    speed = 2.0f;
    direction = glm::vec3(0.0f);

    events::RigidBodyData rigid = {
        glm::vec3(10,0.0f,10), //position
        1, //mass
        capsule, //btshape
        glm::vec3(0,0,0), //inertia
        this //the gameobject
    };
    events::request_rigidbody_event(rigid);
    
    // Lock y-axis
    rigidbody->setLinearFactor(btVector3(1, 0.0f, 1));
    //Lock angular rotation
    rigidbody->setAngularFactor(0);

    setup_listeners();

    transform.set_position(10.0f, 0.0f, 10.0f);
}

void Player::setup_listeners() {
    events::dungeon::player_move_event.connect([&](events::StickData d) {
        to_moveX = d.state.first;
        to_moveZ = d.state.second;
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

    // Test code for playing animation for the boy
    anim_player.set_speed(3.0f);

    //Should change animation stuff. Add a stop
    if (model->animations.size() > 0) {
        anim_player.play(model, "walk");
    }

    do_movement();

    btTransform t;

    // Get the transform from Bullet and into 't'
    rigidbody->getMotionState()->getWorldTransform(t);
    btVector3 to_set = t.getOrigin();

    if (to_set.getX() == NAN) {
        t.setIdentity();
        to_set = t.getOrigin();
    }

    transform.set_position(glm::vec3((float)to_set.getX(), (float)to_set.getY(),
        (float)to_set.getZ()));
}

void Player::do_movement() {
    // Should account for deltatime so movement is
    // framerate independent? Unsure how Bullet handles framerate.
    rigidbody->setActivationState(true);
    rigidbody->setLinearVelocity(btVector3(to_moveX * speed, 0, to_moveZ * speed));
    transform.look_at(glm::vec3(to_moveX * speed, 0, to_moveZ * speed));
    direction = glm::vec3(to_moveX * speed, 0, to_moveZ * speed);
}

void Player::interact() {
    // Asks physics for a raycast to check if the player
    // is facing a construct.
    if(direction.x != 0.0f || direction.z != 0.0f)
        events::dungeon::player_request_raycast_event(transform.get_position(), direction);
}