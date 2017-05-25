#include "gold_ui.h"

#include "events.h"
#include "timer.h"
#include "util/color.h"
#include "asset_loader.h"

#include <string>

GoldUI::GoldUI(float aspect)
        : UI(90.f * aspect, 0.f),
          bg(0, 0, 10.f * aspect, 7.f * aspect, Color::BLACK, 0.5f),
          gold_delta("+0g",
                     2.5f * aspect, 10.f, // starting coordinates
                     5.f * aspect, 5.f, // width and height
                     Color::WHITE),
          gold_image(2.5f * aspect, -1.f,
                     5.f * aspect, 5.f * aspect,
                     AssetLoader::get_texture("essence.png")),
          total_gold("100",
                     2.5f * aspect, 4.f * aspect,
                     5.f * aspect, 5.f,
                     Color::WHITE) {

    attach(bg);

    attach(gold_delta);
    gold_delta.disable(); // not shown by default

    attach(total_gold);
    attach(gold_image);

    events::c_player_stats_updated.connect([&](const proto::PlayerStats &update) {
        set_gold(update.coins());
    });
}

void GoldUI::set_gold(int gold) {
    total_gold.set_text(std::to_string(gold));
}