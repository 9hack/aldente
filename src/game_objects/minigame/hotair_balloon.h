#pragma once

/*
* Balloon class: Balloon part of the hotair balloon minigame
*/

#include "../game_object.h"

enum BalloonColor {
	BALLOON_RED, BALLOON_BLUE,
};

class HotAirBalloon : public GameObject {
private:
	int color;
public:
	HotAirBalloon(int id, BalloonColor to_set);

	void setup_model() override;

	void c_update_state(glm::mat4 mat, bool enab) override;
};