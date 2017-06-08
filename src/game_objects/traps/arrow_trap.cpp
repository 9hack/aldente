#include <audio/audio_manager.h>
#include <game/game_state.h>
#include "arrow_trap.h"
#include "asset_loader.h"

ArrowTrap::ArrowTrap(int x, int z, int id) : ProjectileTrap(x, z, id) {
    if (id == ON_SERVER) {
        // Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box;
        rigid.position = { x, 0.0f, z };
        rigid.is_ghost = true;
        collision_group = COLLISION_STRUCTS;
        events::add_rigidbody_event(rigid);

        fill_projectile_pool(ProjectileTypes::ARROW);

        //activation_type = RAYCAST;
        //raycast_range = 10.f;

        // Fires arrow every ...
        setup_timer(1000);
        activation_type = TIME;
    }
}

void ArrowTrap::setup_model() {
    Model *model = AssetLoader::get_model("arrow_trap");
    attach_model(model);
    set_scale({ 0.0095f, 0.0095f, 0.0095f });

    anim_player.set_anim("shoot");
    anim_player.set_loop(false);
    anim_player.set_speed(5.0f);
}

void ArrowTrap::play_trigger_animation() {
    events::sound_effects_event(events::AudioData(AudioManager::ARROW_SWOOSH_SOUND, false, GameState::context.client_player, this));

    anim_player.play();
}

