#include "ui.h"

#include "shaders/shader_manager.h"

UI::UI(int width, int height)
{
    renderer2D = new Render2D();
    renderer2D->setup_text(width, height);
}

void UI::update()
{
    renderer2D->render_text(ShaderManager::get_shader_program("text"), "Hello world!", 25.f, 25.f, 1.f, glm::vec3(0.5f, 0.8f, 0.2f));
    renderer2D->render_rect(ShaderManager::get_shader_program("text"), 100.f, 100.f, 50.f, 100.f, glm::vec3(0.5f, 0.8f, 0.2f), 0);
}
