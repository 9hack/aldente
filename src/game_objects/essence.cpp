#include "essence.h"
#include "events.h"
#include "asset_loader.h"
#include "player.h"
#include "util/util_bt.h"
#include "timer.h"

#include <iostream>

Essence::Essence(int id) : GameObject(id){
    tag = "ESSENCE";

    // Set intial value
    value = DEFAULT_ESSENSE_VAL;

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
        events::dungeon::network_collision_event(id, 0);
        player->currency.add_gold(value);
        disable(); // Coin can no longer be used and is not drawn. TODO : Garbage Collect
    }
}

void Essence::c_on_collision(int type) {
    disappear();
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

    int count = 1;
    const int max_count = 20;

    // Slowly fade away
    std::function<void()> cancel_fade = Timer::get()->do_every(
        std::chrono::milliseconds(20),
        [&, count, max_count, cancel_fade]() mutable {
        if (count < max_count) {
            set_alpha(1.0f - (max_count / count));
        }
        else {
            cancel_fade();
            disable();
        }

        count++;
    });
}