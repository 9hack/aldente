#include "aldente.h"
#include "shadows.h"
#include "shaders\shader_manager.h"
#include "shaders\shadow_shader.h"
#include "util\util.h"
#include "window.h"
#include "util\config.h"

void Shadows::shadow_pass(Scene* scene)
{	
	float far_plane;
	Config::config->get_value(Config::str_far_plane, far_plane);

	scene->camera->update_frustum_planes();
	scene->camera->update_frustum_corners(Window::width, Window::height, far_plane);

	ShadowShader * ss = (ShadowShader *)ShaderManager::get_shader_program("shadow");
	glViewport(0, 0, ss->size, ss->size);
	glBindFramebuffer(GL_FRAMEBUFFER, ss->FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	ss->use();
	ss->light_pos = scene->light_pos;
	ss->light_proj = scene->camera->frustum_ortho();
	// Render using scene graph.
	glDisable(GL_CULL_FACE);
	scene->pass(ss);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadows::debug_shadows()
{
	glViewport(0, 0, Window::width / 3, Window::height / 3);
	ShaderManager::get_shader_program("debug_shadow")->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ((ShadowShader *)ShaderManager::get_shader_program("shadow"))->shadow_map_tex);
	Util::render_quad();
}