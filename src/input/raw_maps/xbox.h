#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "raw_map.h"
#include "events.h"

namespace input {
    const std::map<RawButton, events::ConceptualButton> BTN_MAP_XBOX {
            // D-Pad
            {{true, 10}, events::BTN_UP},
            {{true, 13}, events::BTN_LEFT},
            {{true, 12}, events::BTN_DOWN},
            {{true, 11}, events::BTN_RIGHT},

            // ABXY
            {{true, 0}, events::BTN_A},
            {{true, 1}, events::BTN_B},
            {{true, 2}, events::BTN_X},
            {{true, 3}, events::BTN_Y},

            // Shoulders
            {{false, 2}, events::AX_LT},
            {{true, 4}, events::BTN_LB},
            {{true, 5}, events::BTN_RB},
            {{false, 2}, events::AX_RT},

            // Stick presses
            {{true, 8}, events::BTN_LS},
            {{true, 9}, events::BTN_RS},

            // Left stick
            {{false, 0}, events::AX_LH},
            {{false, 1}, events::AX_LV},

            // Right stick
            {{false, 4}, events::AX_RH},
            {{false, 3}, events::AX_RV},

            // Back, start
            {{true, 6}, events::BTN_BACK},
            {{true, 7}, events::BTN_START},

            // Xbox button
            {{true, 10}, events::BTN_XBOX},
    };
}
