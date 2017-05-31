#pragma once

#include "ui.h"
#include "ui_text_box.h"

class DebugUI : public UI {
public:
    DebugUI(float aspect);
private:
    UITextBox fps_display;
    UITextBox ping_display;
};