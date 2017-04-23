#include "shadows.h"

#include <tuple>
#include "aldente.h"
#include "shaders/shadow_shader.h"
#include "window.h"
#include "util/config.h"

void Shadows::shadow_pass(Scene *scene) {
    // TODO: disgusting
    int width, height;
    std::tie(width, height) = window.get_size();

    float far_plane;
    Config::config->get_value(Config::str_far_plane, far_plane);

    scene->camera->update_frustum_planes();
    scene->camera->update_frustum_corners(width, height, far_plane);

    ShadowShader *ss = (ShadowShader *) ShaderManager::get_shader_program("shadow");
    glViewport(0, 0, ss->size, ss->size);
    glBindFramebuffer(GL_FRAMEBUFFER, ss->FBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    ss->use();
    ss->light_pos = scene->light_pos;

    // TODO: disgusting
    if (Aldente::shadows_on) {
        ss->light_proj = scene->camera->frustum_ortho(scene->light_pos);
    } else {
        ss->light_proj = glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.f, 0.1f);
    }
    // Render using scene graph.
    glDisable(GL_CULL_FACE);
    scene->pass(ss);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadows::debug_shadows() {
    // TODO: disgusting
    int width, height;
    std::tie(width, height) = window.get_size();

    glViewport(0, 0, width / 3, height / 3);
    ShaderManager::get_shader_program("debug_shadow")->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ((ShadowShader *) ShaderManager::get_shader_program("shadow"))->shadow_map_tex);
    Util::render_quad();
}
