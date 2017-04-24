#include "render.h"

Render::Render(Window &window, SceneManager &scene_manager)
    : window(window), scene_manager(scene_manager) {}

// Render pipeline
// Alternatively can acquire current scene via some ChangeScene event
void Render::update() {
    // First pass: shadowmap.
    shadows.shadow_pass(scene_manager.get_current_scene());

    // Second pass: usual rendering.
    window.clear();
    scene_manager.get_current_scene()->draw();

    shadows.debug_shadows();
}
