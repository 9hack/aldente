#include "pump.h"
#include "asset_loader.h"
#include "events.h"

Pump::Pump(int id, PumpColor to_set) : GameObject(-1) {
    tag = "PUMP";
    color = to_set;
}

void Pump::setup_model() {
    attach_model(AssetLoader::get_model("pump"));
    transform.set_scale({ 0.002f, 0.003f, 0.002f });
    transform.rotate(0, 180, 0);
    initial_transform.set_scale(transform.get_scale());

    anim_player.set_anim("pump");
    anim_player.set_loop(false);
}