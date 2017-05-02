#include "player.h"

// Test code for playing animation for the boy
void Player::update() {
    anim_player.set_speed(3.0f);

    if (model->animations.size() > 0) {
        anim_player.play(model, "walk");
    }
}