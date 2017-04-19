#include "aldente.h"
#include "shadows.h"
#include "shaders\shader_manager.h"
#include "shaders\shadow_shader.h"
#include "util\util.h"

void Shadows::shadow_pass(Scene* scene)
{
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

void Shadows::debug_shadows(int width, int height)
{
	glViewport(0, 0, width / 3, height / 3);
	ShaderManager::get_shader_program("debug_shadow")->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ((ShadowShader *)ShaderManager::get_shader_program("shadow"))->shadow_map_tex);
	Util::render_quad();
}