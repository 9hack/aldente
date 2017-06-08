#include "pump.h"
#include "asset_loader.h"
#include "events.h"

Pump::Pump(int id) : GameObject(-1) {
    tag = "PUMP";
}

void Pump::setup_model() {
    attach_model(AssetLoader::get_model("pump"));
    transform.set_scale({ 0.006f, 0.006f, 0.006f });
    initial_transform.set_scale(transform.get_scale());

    anim_player.set_anim("pump");
    anim_player.set_loop(false);
}