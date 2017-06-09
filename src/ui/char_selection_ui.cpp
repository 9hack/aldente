#include <events.h>
#include "char_selection_ui.h"

#include "timer.h"
#include "util/color.h"

CharSelectionUI::CharSelectionUI(float aspect)
        : UI(0.f, 0.f),
          title(5.f, 5.f, 0.f, -20.f, 100.f * aspect, 100.f, 0.f,
          UIUnstretchedTextBox::MIDDLE, UIUnstretchedTextBox::MIDDLE,
          Color::BLACK, Color::BLACK, 1.f, false) {

    title.set_text("SELECT YOUR CHARACTER");
    title.set_alpha(0.f);
    attach(title);
    disable();

    events::ui::enable_char_selection.connect([&]() {
        enable_animated();
    });

    events::ui::disable_char_selection.connect([&]() {
        disable_animated();
    });

    events::menu::end_menu_event.connect([&]() {
        disable_animated();
    });
}

void CharSelectionUI::enable_animated() {
    enable();
    title.animate_alpha(1.f, 2.f);
}

void CharSelectionUI::disable_animated() {
    title.animate_alpha(0.f, 1.f, [&]() {
        disable();
    });
}