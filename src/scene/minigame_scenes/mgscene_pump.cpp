#include "mgscene_pump.h"
#include "game_objects/player.h"
#include "game_objects/minigame/platform.h"
#include "events.h"
#include "../../asset_loader.h"

void MGScenePump::s_update() {
    Scene::s_update();
}

void MGScenePump::c_update() {
    Scene::c_update();
}

void MGScenePump::s_setup() {
	// Create balloons
	HotAirBalloon *red_balloon = new HotAirBalloon(0, BalloonColor::BALLOON_RED);
	balloons.push_back(red_balloon);
	objs.push_back(red_balloon);
	HotAirBalloon *blue_balloon = new HotAirBalloon(0, BalloonColor::BALLOON_BLUE);
	balloons.push_back(blue_balloon);
	objs.push_back(blue_balloon);

	// Properly space balloons
	balloons[0]->transform.set_position(glm::vec3(-3, 1.6f, 0));
	balloons[0]->transform.set_scale({ INITIAL_SCALE, INITIAL_SCALE, INITIAL_SCALE });
	balloons[1]->transform.set_position(glm::vec3(3, 1.6f, 0));
	balloons[1]->transform.set_scale({ INITIAL_SCALE, INITIAL_SCALE, INITIAL_SCALE });
}

void MGScenePump::c_setup() {
    // Set up the platform
    Platform* platform = new Platform(PlatformShape::SQUARE);

    platform->set_position(glm::vec3(0, -1.f, 0));
    platform->transform.set_scale(glm::vec3(10, 1.0f, 10));
    objs.push_back(platform);

    // Set up 4 pumps
    for (int i = 0; i < 4; i++) {
        Pump *pump;
        if (i <= 1)
            pump = new Pump(-1, PumpColor::RED);
        else
            pump = new Pump(-1, PumpColor::BLUE);

        pumps.push_back(pump);
        objs.push_back(pump);
    }
    
    // Properly space pumps
    pumps[0]->transform.set_position(glm::vec3(-5, 0, 0));
    pumps[1]->transform.set_position(glm::vec3(-1, 0, 0));
    pumps[2]->transform.set_position(glm::vec3(1, 0, 0));
    pumps[3]->transform.set_position(glm::vec3(5, 0, 0));
    

    // Set up the 2 hot air balloons
    for (int i = 0; i < 2; i++) {
        HotAirBasket* basket = new HotAirBasket(-1);
        objs.push_back(basket);
        baskets.push_back(basket);
    }

    // Properly space baskets
    baskets[0]->transform.set_position(glm::vec3(-3, 0, 0));
    baskets[1]->transform.set_position(glm::vec3(3, 0, 0));

	// Create balloons
	HotAirBalloon *red_balloon = new HotAirBalloon(0, BalloonColor::BALLOON_RED);
	balloons.push_back(red_balloon);
	objs.push_back(red_balloon);
	HotAirBalloon *blue_balloon = new HotAirBalloon(0, BalloonColor::BALLOON_BLUE);
	balloons.push_back(blue_balloon);
	objs.push_back(blue_balloon);

	// Properly space balloons
	balloons[0]->transform.set_position(glm::vec3(-3, 1.6f, 0));
	balloons[1]->transform.set_position(glm::vec3(3, 1.6f, 0));

    // Setup lights.
    DirectionalLight *sun = new DirectionalLight(glm::vec3(0.f, -1.f, -1.f),
        Color::WHITE, 0.5f);
    add_light(sun);

    for (GameObject *obj : objs) {
        obj->setup_model();
    }

	pump_conn = events::minigame::c_assign_pump_event.connect([&](int player_id, int pump) {
		//pump_map[player_id] = pumps[pump];
		std::cerr << "[c] assign pump " << pump << " to player " << player_id << "\n";
		pumps[pump]->c_set_player_id(player_id);
	});
}

void MGScenePump::connect_listeners() {
    
}

void MGScenePump::disconnect_listeners() {
    pump_conn.disconnect();
}

void MGScenePump::reset_camera() {
    info.camera.reset();
    info.camera.rotate_cam(glm::vec3(1, 0, 0), -45.0f);
    info.camera.cam_pos = glm::vec3(0, 8, 9);
    info.camera.recalculate();
}

void MGScenePump::reset_scene() {
	balloons[0]->reset();
	balloons[1]->reset();
}

void MGScenePump::inflate_balloon(bool is_team1) {
    if (is_team1)
        balloons[0]->inflate();
    else
        balloons[1]->inflate();
}

void MGScenePump::c_add_pump(int count, int player_id) {
    //pump_map[player_id] = pumps[count];
}

void MGScenePump::c_trigger_pump(int player_id) {
    // TODO: play animation of this pump:
    // pump_map[player_id]
}