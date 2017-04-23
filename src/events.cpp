#include "events.h"

namespace events {
    using boost::signals2::signal;
    signal<void(JoystickData &)> joystick_event;
    signal<void(WindowSizeData &)> window_resize_event;
    signal<void(WindowKeyData &)> window_key_event;
    signal<void(WindowCursorData &)> window_cursor_event;
    signal<void(WindowMouseButtonData &)> window_mouse_button_event;
    signal<void(WindowScrollData &)> window_scroll_event;
}
