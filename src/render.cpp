#include "render.h"

Render::Render(SceneManager &scene_manager)
    : scene_manager(scene_manager) {}

// Render pipeline
// Alternatively can acquire current scene via some ChangeScene event
void Render::update() {
    // First pass: shadowmap.
    shadows.shadow_pass(scene_manager.get_current_scene());

    // Second pass: usual rendering.
    scene_manager.get_current_scene()->draw();

    // Debug shadows as necessary.
    shadows.debug_shadows();
}
