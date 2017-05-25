#include "slime.h"
#include "asset_loader.h"
#include "util/color.h"

Slime::Slime(int x, int z, int id) : MobileTrap(x, z, id) {
    if (id == ON_SERVER) {
        //Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box_small;
        rigid.mass = 1;
        rigid.is_ghost = false;
        rigid.position = { x, 0.0f, z };
        events::add_rigidbody_event(rigid);

        // Lock y-axis
        rigidbody->setLinearFactor(btVector3(1, 0.0f, 1));
    }
}

SlimeBlue::SlimeBlue(int x, int z, int id) : Slime(x, z, id) {
    if (id == ON_SERVER) {
        // Move Speed
        move_speed = 2.0f;

        // Time movement based
        move_type = TIME;
        setup_timer(6000); // changes direction every 6 seconds

        rotation_amount = -90.0f;
    }
}



SlimeYellow::SlimeYellow(int x, int z, int id) : Slime(x, z, id) {
    if (id == ON_SERVER) {

        set_ghost(true);

        // Move Speed
        move_speed = 2.0f;

        move_type = WALL;

        random_rotations_on = true;
    }
}

SlimeRed::SlimeRed(int x, int z, int id) : Slime(x, z, id) {
    if (id == ON_SERVER) {

        set_ghost(true);

        // Move Speed
        move_speed = 4.0f;

        move_type = WALL;
        rotation_amount = 180.0f;
    }
}

SlimeGreen::SlimeGreen(int x, int z, int id) : Slime(x, z, id) {
    if (id == ON_SERVER) {
        // Move Speed
        move_speed = 2.0f;

        // Time movement based
        move_type = TIME;
        setup_timer(3000); // changes direction every 3 seconds

        // Rotation Amount
        random_rotations_on = true;
    }
}

void SlimeBlue::setup_model() {
    setup_slime_model(std::string("slime_blue"));
}

void SlimeYellow::setup_model() {
    setup_slime_model(std::string("slime_yellow"));
}

void SlimeRed::setup_model() {
    setup_slime_model(std::string("slime_red"));
}

void SlimeGreen::setup_model() {
    setup_slime_model(std::string("slime_green"));
}

void Slime::setup_slime_model(std::string &slime_name) {
    Model *model = AssetLoader::get_model(slime_name);
    attach_model(model);
    transform.set_scale({ 0.004f, 0.004f, 0.004f });

    anim_player.set_anim("walk");
    anim_player.set_loop(true);
    anim_player.play();
}