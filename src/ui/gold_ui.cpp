#include "gold_ui.h"

#include "events.h"
#include "timer.h"
#include "util/color.h"
#include "asset_loader.h"

#include <string>

GoldUI::GoldUI(float aspect)
        : UI(83.f * aspect, 0.f),
          bg(0, 0, 10.f * aspect, 5.f * aspect, Color::BLACK, 0.f), // not shown!
          gold_delta(3.f, 4.f,
                     0.f, 0.f, 
                     12.f * aspect, 7.f * aspect, // Same-ish size as BG for convenience
                     0.f, UIUnstretchedTextBox::END, UIUnstretchedTextBox::MIDDLE,
                     Color::WHITE, Color::BLACK, 0.f, false),
          gold_image(12.5f * aspect, 0.5f,
                     4.f * aspect, 4.f * aspect,
                     AssetLoader::get_texture("essence.png")),
          total_gold(3.f, 4.f,
                     0.f * aspect, -5.f * aspect, // random ass values
                     12.f * aspect, 7.f * aspect, 0.f, // it works
                     UIUnstretchedTextBox::END, UIUnstretchedTextBox::START,
                     Color::WHITE, Color::BLACK, 0.f, false),
          cur_gold(0),
          cur_delta(0),
          delta_showing(false),
          cancel_delta_out([](){}) {

    attach(bg);

    attach(total_gold);
    attach(gold_image);

    // Hide at first
    gold_delta.set_alpha(0.f);
    attach(gold_delta);

    // Disabled at first. Shown in build phase.
    disable();

    events::menu::end_menu_event.connect([&]() {
        enable();
    });

    events::end::start_end_event.connect([&]() {
        disable();
    });

    events::c_player_stats_updated.connect([&](const proto::PlayerStats &update) {
        set_gold(update.coins());
    });
}

void GoldUI::set_gold(int gold) {
    // If delta not showing, animate it in and prepare to animate it out
    if (!delta_showing) {
        delta_showing = true;
        cur_delta = 0;
        gold_delta.animate_alpha(1.f, .25f);
        gold_delta.animate_relative(0, 2.f, .25f, [this]() {
            fade_delta_after(500);
        });
    } else {
        fade_delta_after(500);
    }

    // Update amounts
    cur_delta += gold - cur_gold;
    cur_gold = gold;
    total_gold.set_text(std::to_string(cur_gold));

    // Update delta text
    std::string prefix = cur_delta <= 0 ? "-" : "+";
    Color color = cur_delta < 0 ? Color::INDIAN_RED : Color::WINDWAKER_GREEN;
    gold_delta.set_text(prefix + std::to_string(std::abs(cur_delta)));
    gold_delta.set_color(color);
}

void GoldUI::fade_delta_after(int ms) {
    // Cancel any existing fade state
    cancel_delta_out();

    // To cancel the existing animation, if any
    gold_delta.animate_alpha(1.f, .01f, [this]() {
        cancel_delta_out = Timer::get()->do_after(std::chrono::milliseconds(500), [this]() {
            gold_delta.animate_alpha(0.f, .1f, [this]() {
                // Reset the delta
                delta_showing = false;
                gold_delta.set_start_x(0);
                gold_delta.set_start_y(0);
            });
        });
    });
}