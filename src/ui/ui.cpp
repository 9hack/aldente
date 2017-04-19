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
    renderer2D->render_rectP(ShaderManager::get_shader_program("text"), 0.1f, 0.1f, 0.5f, 0.5f, glm::vec3(226.f, 122.f, 61.f) / 255.f, 0);

	// Loop through zbuffers.
	for (unsigned int i = 0; i <= zdepth; ++i)
	{
		//zbuffers.find
	}
}
