#include "ball.h"
#include "asset_loader.h"
#include "events.h"

Ball::Ball(int id) : GameObject(id) {
    tag = "BALL";
}

void Ball::s_update_this() {
    if (player) {
        transform.set_position(player->transform.get_position());
    }
}

void Ball::c_update_state(glm::mat4 mat, bool enab) {
    anim_player.update();
    GameObject::c_update_state(mat, enab);
}

void Ball::setup_model() {
}

void Ball::c_set_player(Player *to_set) {
    player = to_set;
    int index = player->c_get_model_index();

    attach_model(AssetLoader::get_model(BALL_MODELS[index]));
    transform.set_scale({ 0.006f, 0.006f, 0.006f });
    initial_transform.set_scale(transform.get_scale());
}
