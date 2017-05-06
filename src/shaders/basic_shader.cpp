#include "basic_shader.h"

#include "util/color.h"
#include "shader_manager.h"
#include "scene/directional_light.h"
#include "scene/point_light.h"
#include "scene/spot_light.h"

void BasicShader::init() {
    // Nothing to be done. For now.
}

void BasicShader::pre_draw(SceneInfo &scene_info) {
    // Nothing to be done. For now.
}

void BasicShader::post_draw() {
    // Nothing to be done. For now.
}

void BasicShader::draw(Mesh *mesh, SceneInfo &scene_info, glm::mat4 to_world) {
    /* MESH UNIFORMS */
    // Send mesh local transformation matrix.
    set_uni("mesh_model", mesh->local_transform);

    // Send material.
    set_uni("material.diffuse", mesh->material->diffuse.to_vec());
    set_uni("material.specular", mesh->material->specular.to_vec());
    set_uni("material.ambient", mesh->material->ambient.to_vec());
    set_uni("material.shininess", mesh->material->shininess);
    set_uni("shadows_enabled", mesh->material->shadows);

    // Send texture uniforms.
    set_uni("texture_enabled", mesh->geometry->has_texture);
    // Bind Texture.
    if (mesh->geometry->has_texture) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mesh->geometry->texture);
        set_uni("texture_map", 1); // ID of this texture=1
    }

    /* SHADOW MATRICES */
    // Send shadow uniforms and bind its texture.
    set_uni("light_matrix", ShaderManager::shadow.light_matrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ShaderManager::shadow.shadow_map_tex);
    set_uni("shadow_map", 0); // ID of this texture=0

    /* LIGHTS */
    // Send directional lights.
    std::string uni_prefix = "dir_lights[";
    set_uni("num_dir_lights", (int) scene_info.dir_lights.size());
    for (int i = 0; i < scene_info.dir_lights.size(); ++i) {
        DirectionalLight l = scene_info.dir_lights[i];
        std::string idx = std::to_string(i);
        set_uni(uni_prefix + idx + "].direction", l.get_direction());
        set_uni(uni_prefix + idx + "].color", l.color.to_vec());
        set_uni(uni_prefix + idx + "].ambient_coeff", l.ambient_coeff);
        set_uni(uni_prefix + idx + "].intensity", l.intensity);
    }

    // Send point lights.
    set_uni("num_point_lights", (int) scene_info.point_lights.size());
    uni_prefix = "point_lights[";
    for (int i = 0; i < scene_info.point_lights.size(); ++i) {
        PointLight l = scene_info.point_lights[i];
        std::string idx = std::to_string(i);
        set_uni(uni_prefix + idx + "].position", l.get_position());
        set_uni(uni_prefix + idx + "].color", l.color.to_vec());
        set_uni(uni_prefix + idx + "].ambient_coeff", l.ambient_coeff);
        set_uni(uni_prefix + idx + "].intensity", l.intensity);
        set_uni(uni_prefix + idx + "].quadratic", l.quadratic);
    }

    // Send spot lights.
    set_uni("num_spot_lights", (int) scene_info.spot_lights.size());
    uni_prefix = "spot_lights[";
    for (int i = 0; i < scene_info.spot_lights.size(); ++i) {
        SpotLight l = scene_info.spot_lights[i];
        std::string idx = std::to_string(i);
        set_uni(uni_prefix + idx + "].position", l.get_position());
        set_uni(uni_prefix + idx + "].direction", l.get_direction());
        set_uni(uni_prefix + idx + "].color", l.color.to_vec());
        set_uni(uni_prefix + idx + "].angle", l.angle);
        set_uni(uni_prefix + idx + "].taper", l.taper);
        set_uni(uni_prefix + idx + "].intensity", l.intensity);
        set_uni(uni_prefix + idx + "].quadratic", l.quadratic);
        set_uni(uni_prefix + idx + "].ambient_coeff", l.ambient_coeff);
    }

    /* TRANSFORMATION MATRICES */
    // Send camera position uniform.
    set_uni("cam_pos", scene_info.camera.cam_pos);
    // Send projection and view matrices of camera.
    set_uni("projection", scene_info.camera.P);
    set_uni("view", scene_info.camera.V);

    // Send model_to_world matrix.
    set_uni("model", to_world);

    /* ALL DONE. START DRAWING */
    // Bind mesh geometry and draw.
    mesh->geometry->bind();
    mesh->geometry->draw();
    // Unbind.
    glBindVertexArray(0);
}
