#include "build_ui.h"

#include "util/color.h"
#include "events.h"
#include "asset_loader.h"

#include "timer.h"

BuildUI::BuildUI(int num_cols, int num_rows, float aspect, std::vector<ConstructData>& constructs)
    : UI(), // explicit call base class dflt constructor
      constructs(constructs),
      ui_grid(0, 0, 30.f * aspect, 70.f, num_cols*num_rows, num_cols, 12, 12, Color::LOZ_GREEN, 3, 1.f),
      rect(0, 0, 12, 12, Color::LOZ_LIGHT_GREEN),
      info_panel(0, 80.f),
      player_panel(0, 0),
      shop_panel(0, 10.f),
      info_rect(0, 0, 30.f * aspect, 20.f, Color::LOZ_DARK_GREEN),
      player_rect(0, 0, 30.f * aspect, 10.f, Color::LOZ_DARK_GREEN),
      title_label("Select a block...", 2.f * aspect, 9.f, 19.f * aspect, 9.f, Color::WHITE),
      description_label("", 2.f * aspect, 2.f, 26.f * aspect, 5.f, Color::WHITE),
      cost_label("0", 23.f * aspect, 9.f, 5.f * aspect, 9.f, Color::WHITE),
      balance_label("100g", 9.f * aspect, 2.f, 12.f * aspect, 6.f, Color::WHITE) {

    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            ui_grid.attach_at(i, j, rect);

            UIImageNode* item_image = new UIImageNode(1, 1, 10, 10,
                AssetLoader::get_texture(constructs[i * num_cols + j].image));
            ui_grid.attach_at(i, j, *item_image);
            images.push_back(item_image);
        }
    }

    shop_panel.attach(ui_grid);
    attach(shop_panel);

    info_panel.attach(info_rect);
    info_panel.attach(title_label);
    info_panel.attach(description_label);
    info_panel.attach(cost_label);
    attach(info_panel);

    player_panel.attach(player_rect);
    player_panel.attach(balance_label);
    attach(player_panel);

    // Display info of first element by default.
    update_info_panel(0);

    events::ui_grid_movement_event.connect([&](int content_index) {
        update_info_panel(content_index);
    });

    events::ui_grid_selection_event.connect([&](int content_index) {
        events::build::c_check_funds_event(this->constructs[content_index].type);
    });

    // Show or hide the grid.
    events::build::construct_selected_event.connect([&](ConstructType type) {
        shop_panel.disable();
        player_panel.disable();
    });
    events::build::select_grid_return_event.connect([&]() {
        shop_panel.enable();
        player_panel.enable();
    });

    // Update the player's current gold balance.
    events::c_player_coins_update_event.connect([&](int coins) {
        std::string s = std::to_string(coins) + "g";
        balance_label.set_text(s);
    });
}

BuildUI::~BuildUI() {
    for (UIImageNode* image : images)
        delete image;
}

void BuildUI::update_info_panel(int content_index) {
    title_label.set_text(constructs[content_index].name);
    description_label.set_text(constructs[content_index].description);
    std::string s = std::to_string(constructs[content_index].cost) + "g";
    cost_label.set_text(s);
}
