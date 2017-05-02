#include "test_chest.h"

// Test code for playing animation for the boy
void TestChest::update() { 
    if (model->animations.size() > 0) {
        anim_player.play(model, "open");
    }
}