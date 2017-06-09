#include "hotair_balloon.h"
#include "asset_loader.h"
#include "events.h"

HotAirBalloon::HotAirBalloon(int id, BalloonColor to_set) : GameObject(id) {
	tag = "BALLOON";
	color = to_set;
}

void HotAirBalloon::setup_model() {
	switch (color) {
		case BalloonColor::BALLOON_RED:
			attach_model(AssetLoader::get_model("red_balloon"));
			break;
		case BalloonColor::BALLOON_BLUE:
			attach_model(AssetLoader::get_model("blue_balloon"));
			break;
		default: break;
	}
	//model->set_shader(&ShaderManager::unlit);
	transform.set_scale({ 0.001f, 0.001f, 0.001f });
	initial_transform.set_scale(transform.get_scale());
}

void HotAirBalloon::c_update_state(glm::mat4 mat, bool enab) {
	if (enab)
		enable();
	else
		disable();

	if (enabled) {
		transform.set_world_mat(mat);
		set_rb_transform();
	}
}