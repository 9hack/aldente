#include "player.h"

// Test code for playing animation for the boy
void Player::update() {
    anim_player.update();
}

void Player::start_walk() {
    anim_player.set_speed(3.0f);
    anim_player.set_anim(model, "walk");
    anim_player.set_loop(true);
    anim_player.play();
}

void Player::stop_walk() {
    anim_player.stop();
}