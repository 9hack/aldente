#include "shadow_shader.h"

#include <glm/gtc/matrix_transform.hpp>

void ShadowShader::init() {
    // Generate new framebuffer for the shadow shader to render to.
    // The new framebuffer use a 2D texture as the depth buffer.
    glGenFramebuffers(1, &FBO);

    // Generate shadow map texture.
    shadow_map_size = 4096;
    glGenTextures(1, &shadow_map_tex);
    glBindTexture(GL_TEXTURE_2D, shadow_map_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 shadow_map_size, shadow_map_size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // Borders are clamped to 1.0f so that coordinates outside of range will have no shadows.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Attach texture as framebuffer's depth buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map_tex, 0);
    // Don't draw to colour buffer.
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowShader::pre_draw(SceneInfo &scene_info) {
    // Set resolution of shadow map.
    glViewport(0, 0, shadow_map_size, shadow_map_size);
    // Render to the shadow shader's FBO.
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    // Disable back face culling for shadow accuracy
    glDisable(GL_CULL_FACE);

    // Calculate light projection matrix to use based on frustum intersection
    glm::mat4 light_proj = // HARD-CODED!
            glm::ortho(-20.f, 60.f, -20.f, 20.f, -30.f, 30.f); //scene_info.camera->frustum_ortho(scene_info.light_pos);
    // Recalculate light matrix based on current light position and light projection matrix
    // Use the first directional light. TODO: multiple light shadows.
    if (scene_info.dir_lights.size() == 0) return;
    glm::mat4 light_view = glm::lookAt(-scene_info.dir_lights[0].get_direction(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    light_matrix = light_proj * light_view;

    // Send updated light matrix as uniform.
    set_uni("view_projection", light_matrix);
}

void ShadowShader::post_draw() {
    // Re-enable back face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowShader::draw(Mesh *mesh, SceneInfo &scene_info, glm::mat4 to_world) {
    set_uni("mesh_model", mesh->local_transform);
    set_uni("model", to_world);
    mesh->geometry->bind();
    mesh->geometry->draw();
    glBindVertexArray(0);
}
