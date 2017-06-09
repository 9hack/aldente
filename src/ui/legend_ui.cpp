#include "legend_ui.h"
#include "events.h"

// Legend presets
static const LegendUI::str_pair BUILD_MENU_LEGEND = {
        {"xboxControllerDPad.png", "Move selection"},
        {"xboxControllerButtonA.png", "Choose"},
        {"xboxControllerStart.png", "Ready up"},
        {"xboxControllerLeftShoulder.png", "Leaderboard"},
};
static const LegendUI::str_pair BUILD_SELECTED_LEGEND = {
        {"xboxControllerLeftThumbstick.png", "Select location"},
        {"xboxControllerButtonA.png", "Build"},
        {"xboxControllerButtonB.png", "Cancel"},
        {"xboxControllerLeftShoulder.png", "Rotate CW"},
        {"xboxControllerRightShoulder.png", "Rotate CCW"},
};
static const LegendUI::str_pair CHAR_SELECTION_LEGEND = {
    {"xboxControllerStart.png", "Confirm"},
    {"xboxControllerLeftShoulder.png", "Switch character"},
    {"xboxControllerRightShoulder.png", "Switch character"},
};

LegendUI::LegendUI(float aspect, float legend_width_, float entry_height_, float padding_) :
        UI((100.f - (legend_width_ + padding_)) * aspect, 100.f),
        legend_width(legend_width_ * aspect),
        entry_height(entry_height_),
        padding(padding_) {

    disable(); // Disable by default.

    // Show the build menu legends when relevant.
    events::ui::show_legend.connect([this](const str_pair &legend) {
        set_legend(legend);
        enable();
    });
    events::ui::dismiss_legend.connect([this]() { disable(); });

    // Legacy legend stuff.
    events::build::select_grid_return_event.connect([&]() {
        set_legend(BUILD_MENU_LEGEND);
        enable();
    });
    events::build::construct_selected_event.connect([&](auto _) {
        set_legend(BUILD_SELECTED_LEGEND);
        enable();
    });
    events::build::end_build_event.connect([&]() {
        disable();
    });

    // New legacy legend stuff.
    events::ui::enable_char_selection.connect([&]() {
        set_legend(CHAR_SELECTION_LEGEND);
        enable();
    });
    events::menu::end_menu_event.connect([&]() {
        disable();
    });
}

void LegendUI::set_legend(const str_pair &legend_spec) {
    // Detach all previous legends
    for (auto &legend : legends)
        detach(*legend);
    legends.clear();

    // Adjust the starting y
    float start_y = 100.f - (entry_height + padding) * legend_spec.size();
    root.set_start_y(start_y);

    // Create the legend elements
    float legend_y = entry_height * (legend_spec.size() - 1);
    for (int i = 0; i < legend_spec.size(); ++i) {
        const auto &legend = legend_spec[i];
        legends.push_back(
                std::make_unique<UILegend>(
                        legend.first, legend.second,
                        0, legend_y,
                        legend_width, entry_height, padding,
                        Color::WHITE, Color::BLACK, 0.5f));
        attach(*legends.back());
        legend_y -= entry_height + padding;
    }
}
