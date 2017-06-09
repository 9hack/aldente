#include "pump.h"
#include "asset_loader.h"
#include "events.h"

Pump::Pump(int id, PumpColor to_set) : GameObject(-1) {
    tag = "PUMP";
    color = to_set;

    events::minigame::c_play_pump_event.connect([&](int id_) {
        if (id_ == player_id) {
            anim_player.stop();
            anim_player.play();
        }
    });
}

void Pump::setup_model() {
    switch (color) {
        case PumpColor::RED:
            attach_model(AssetLoader::get_model("pump_red"));
            break;
        case PumpColor::BLUE:
            attach_model(AssetLoader::get_model("pump_blue"));
            break;
        default: break;
    }
    transform.set_scale({ 0.002f, 0.0015f, 0.002f });
    initial_transform.set_scale(transform.get_scale());

    anim_player.set_anim("pump");
    anim_player.set_speed(2.0f);
    anim_player.set_loop(false);
}