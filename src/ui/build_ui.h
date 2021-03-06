#pragma once

#include "ui.h"

#include "ui_grid.h"
#include "ui_rectangle.h"
#include "ui_text_box.h"
#include "ui_image_node.h"
#include "game/construct_types.h"

class BuildUI : public UI {
public:
    BuildUI(int num_cols, int num_rows, float aspect, std::vector<ConstructData>& constructs);
    ~BuildUI();
private:
    // Displays the construct's (of the given index) info in the display panel.
    void update_info_panel(int content_index);

    std::vector<ConstructData> constructs;
    UIGrid ui_grid;
    UIRectangle rect;
    UIContainer info_panel, shop_panel;
    UIRectangle info_rect;
    UITextBox title_label, description_label, cost_label;
    UIImageNode gold_image;

    std::vector<UIImageNode*> images;
};
