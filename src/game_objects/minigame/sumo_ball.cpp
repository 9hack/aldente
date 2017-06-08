#include "sumo_ball.h"
#include "asset_loader.h"
#include "events.h"
#include "util/util_bt.h"

SumoBall::SumoBall(int id) : GameObject(id) {
    tag = "BALL";
    first = true;
}

void SumoBall::c_update_this() {
    // Make the ball follow the player and
    // rotate with its direction
    if (player) {
        glm::vec3 pos = player->transform.get_position();
        transform.set_position(pos.x, pos.y - 0.5f, pos.z);

        if (first) {
            prev_player_vec = pos;
            first = false;
            return;
        }

        glm::vec3 vec = pos - prev_player_vec;
        vec.y = 0;
        float speed = glm::length(vec);

        if (speed != 0) {
            glm::vec3 axis = glm::cross(glm::vec3(0, 1, 0), glm::normalize(vec));
            axis = glm::normalize(axis);
            transform.rotate(axis, speed * 100.f);
        }

        prev_player_vec = pos;
    }
}

void SumoBall::c_set_player(Player *to_set) {
    player = to_set;
    int index = player->c_get_model_index();

    attach_model(AssetLoader::get_model(BALL_MODELS[index]));
    transform.set_scale({ 0.006f, 0.006f, 0.006f });
    initial_transform.set_scale(transform.get_scale());
}
