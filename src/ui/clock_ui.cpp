#include <events.h>
#include "clock_ui.h"

#include "timer.h"
#include "util/color.h"

ClockUI::ClockUI(float aspect)
    : clock(40.f * aspect, 90.f, 20.f * aspect, 10.f, Color::WHITE, Color::BLACK) {
    attach(clock);

    events::ui::update_time.connect([&](int time) {
        clock.set_time(time);
    });
}


