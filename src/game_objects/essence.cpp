#include "essence.h"
#include "events.h"
#include "asset_loader.h"
#include "player.h"
#include "util/util_bt.h"
#include "timer.h"
#include "util/util.h"

#include <iostream>

Essence::Essence(int id) : GameObject(id){
    tag = "ESSENCE";

    // Set intial value
    value = DEFAULT_ESSENSE_VAL;

    if (id == ON_SERVER) {
        // Setup rigid body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_sphere;
        rigid.mass = 1;
        events::add_rigidbody_event(rigid);
        notify_on_collision = true;

        // Lock y-axis
        rigidbody->setLinearFactor(btVector3(1, 0.0f, 1));

        //random_push();
    }
    else {
        // Make the essence change colors continuously
        int num_steps = 200;
        int count = (int) Util::random(0, num_steps * 2);
        cancel_rainbow = Timer::get()->do_every(
            std::chrono::milliseconds(10),
            [&, num_steps, count]() mutable{
            const float frequency = (2 * glm::pi<float>()) / num_steps;
            float r, g, b;

            // For smooth looping
            int i = count;
            if (count > num_steps)
                i = num_steps * 2 - count;

            // Adjust rgb values based on frequencies, the last number dictates the 'balance' (0 - 255)
            r = sinf(frequency * i + 0) * 127 + 200;
            g = sinf(frequency * i + 2) * 127 + 200;
            b = sinf(frequency * i + 4) * 127 + 200;

            set_filter_color({ r, g, b });

            count = (count < num_steps * 2) ? count + 1 : 0;
        });
    }
}

void Essence::s_on_collision(GameObject *other) {
    Player *player = dynamic_cast<Player*>(other);
    if (player) {
        return;
        events::dungeon::network_collision_event(id, 0);
        player->currency.add_gold(value);
        // TODO : Call disable() once client side animation ends
    }
}

void Essence::c_on_collision(int type) {
    disappear();
}

void Essence::setup_model() {
    attach_model(AssetLoader::get_model("essence"));
    transform.set_scale({ 0.005f, 0.005f, 0.005f });

    // Coin always spins
    anim_player.set_anim("spin");
    anim_player.set_speed(Util::random(0.4f, 2.0f));
    anim_player.set_loop(true);
    anim_player.play();
}

void Essence::disappear() {

    int count = 1;
    // Slowly fade away
    cancel_fade = Timer::get()->do_every(
        std::chrono::milliseconds(50),
        [&, count]() mutable {
        const int max_count = 20;

        if (count < max_count) {
            set_filter_alpha(1.0f - (count / max_count));
        }
        else {
            cancel_fade();
        }

        count++;
    });
}

void Essence::random_push() {
    // Setup Initial Push in Random direction
    glm::vec2 vel = glm::vec2(Util::random(-1.f, 1.f), Util::random(-1.f, 1.f));
    vel = vel * Util::random(1.f, 3.f); // Random Force

    rigidbody->setActivationState(true);
    rigidbody->setLinearVelocity(btVector3(vel.x, 0, vel.y));
}