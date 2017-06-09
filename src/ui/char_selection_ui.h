#pragma once

#include "ui.h"
#include "ui_text_box.h"
#include "ui_grid.h"
#include "ui_rectangle.h"
#include "ui_unstretched_text_box.h"

class CharSelectionUI : public UI {
public:
    CharSelectionUI(float aspect);
    void enable_animated() override;
    void disable_animated() override;
private:
    UIUnstretchedTextBox title;
};