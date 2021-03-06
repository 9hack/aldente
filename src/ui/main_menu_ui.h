#pragma once

#include "ui.h"
#include "ui_rectangle.h"
#include "ui_unstretched_text_box.h"
#include "ui_image_node.h"

#include <boost/signals2.hpp>

class MainMenuUI : public UI {
public:
    MainMenuUI(float aspect);
    void enable_animated() override;
    void disable_animated() override;
private:
    boost::signals2::connection button_conn;
    std::function<void()> press_anim;

    UIRectangle bg;
    UIUnstretchedTextBox press;
    UIImageNode logo;
};