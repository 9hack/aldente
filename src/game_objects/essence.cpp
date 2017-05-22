#include "essence.h"
#include "events.h"
#include "asset_loader.h"
#include "player.h"
#include "util/util_bt.h"

#include <iostream>

Essence::Essence(int id) : GameObject(id){
    tag = "ESSENCE";

    // Set intial value
    value = DEFAULT_VALUE;

    // Setup rigid body
    events::RigidBodyData rigid;
    rigid.object = this;
    rigid.shape = hit_sphere;
    events::add_rigidbody_event(rigid);
    notify_on_collision = true;

    // Makes rigid body moveable
    //get_rigid()->setActivationState(true);
}

void Essence::s_on_collision(GameObject *other) {
    Player *player = dynamic_cast<Player*>(other);
    if (player) {
        std::cerr << "Player picked up a coin" << std::endl;
        get_rigid()->setLinearVelocity(btVector3(5.0f, 5.0f, 5.0f));
    }
}

void Essence::c_on_collision(int type) {

}

void Essence::setup_model() {
    attach_model(AssetLoader::get_model("essence"));
    transform.set_scale({ 0.01f, 0.01f, 0.01f });

    // Coin always spins
    anim_player.set_anim("spin");
    anim_player.set_loop(true);
    anim_player.play();
}

void Essence::disappear() {

}