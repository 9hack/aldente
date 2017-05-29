#include "fps_ui.h"

#include "events.h"

DebugUI::DebugUI(float aspect)
    : UI(95.f * aspect, 95.f),
      fps_display("0",
                  0, 0,
                  5.f * aspect, 5.f,
                  Color::WHITE, Color::BLACK, 0.5f),
      ping_display("0",
                   -5.f, -5.f,
                   5.f * aspect + 5.f, 5.f,
                   Color::GREEN, Color::BLACK, 0.5f) {

    attach(fps_display);
    attach(ping_display);

    // not shown by default
    disable();

    events::debug::toggle_fps_event.connect([&]() {
        toggle();
    });

    events::debug::fps_count_event.connect([&](int fps) {
        fps_display.set_text(std::to_string(fps));
    });

    events::debug::ping_changed_event.connect([&](long long ping) {
        ping_display.set_text(std::to_string(ping) + "ms");
        if (ping < 30)
            ping_display.set_text_color(Color::GREEN);
        else if (ping < 60)
            ping_display.set_text_color(Color::YELLOW);
        else if (ping < 100)
            ping_display.set_text_color(Color::AUTUMN_ORANGE);
        else
            ping_display.set_text_color(Color::RED);
    });
}