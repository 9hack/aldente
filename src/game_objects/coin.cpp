#include "coin.h"
#include "events.h"
#include "asset_loader.h"
#include "player.h"
#include "util/util_bt.h"

#include <iostream>

Coin::Coin(int val) : GameObject(){
    // Set intial value
    value = val;

    notify_on_collision = true;

    // Setup rigid body
    events::RigidBodyData rigid;
    rigid.object = this;
    rigid.shape = hit_sphere;
    events::add_rigidbody_event(rigid);

    // Makes rigid body moveable
    get_rigid()->setActivationState(true);
}

void Coin::update_this() {
    anim_player.update();

    // Get the transform from Bullet and into 't'
    btTransform t;
    rigidbody->getMotionState()->getWorldTransform(t);
    btVector3 to_set = t.getOrigin();
    transform.set_position(util_bt::convert_vec3(to_set));

}

void Coin::on_collision(GameObject *other) {
    Player *player = dynamic_cast<Player*>(other);
    if (player) {
        std::cerr << "Player picked up a coin" << std::endl;
        get_rigid()->setLinearVelocity(btVector3(5.0f, 5.0f, 5.0f));
    }
}

void Coin::setup_model() {
    attach_model(AssetLoader::get_model("coin"));
    transform.set_scale({ 0.01f, 0.01f, 0.01f });

    // Coin always spins
    anim_player.set_anim(&skel, "spin");
    anim_player.set_loop(true);
    anim_player.play();
}