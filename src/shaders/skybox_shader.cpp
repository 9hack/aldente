#include "skybox_shader.h"

#include "model/geometry_generator.h"

#include "events.h"
#include "timer.h"
#include "util/util.h"

#define SKYBOX_DIRECTORY_PATH "assets/skybox/"

// List of skyboxes. Hard-coded for now.
std::vector<std::string> SkyboxShader::skyboxes = {
        "cloud0", "cloud1",
        "cloud2", "cloud3",
        "cloud4",
        "fog", "space"
};

void SkyboxShader::set_skybox(std::string skybox_name) {
    if (texture_ids.find(skybox_name) != texture_ids.end())
        current_texture_id = texture_ids[skybox_name];
}

void SkyboxShader::init() {
    // Use cube geometry.
    cube_geometry = GeometryGenerator::generate_cube(2.f);

    // Name of each file, in ppm format. Hard-coded for now.
    std::vector<std::string> faces {
            "/right.ppm", "/left.ppm",
            "/top.ppm", "/bottom.ppm",
            "/back.ppm", "/front.ppm"
    };

    int tex_width, tex_height;
    unsigned char *tex_image;

    // Load and create textures for all skyboxes.
    for (std::string s : skyboxes) {
        GLuint id;
        glGenTextures(1, &id);
        texture_ids[s] = id;
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);

        // Load each face's texture.
        for (unsigned int i = 0; i < faces.size(); ++i) {
            std::string path = SKYBOX_DIRECTORY_PATH + s + faces[i];
            tex_image = Util::loadPPM(path.c_str(), tex_width, tex_height);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB,
                         tex_width, tex_height,
                         0, GL_RGB, GL_UNSIGNED_BYTE,
                         tex_image);
            delete tex_image;
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    // Set default skybox to be the first one.
    set_skybox(skyboxes[0]);

    // Listen to round changing.
    events::ui::round_changed_event.connect([&](int round_number) {
       set_skybox(skyboxes[round_number % 5]);
    });

    events::menu::end_menu_event.connect([&]() {
        rot_angle = 0.f;
        rot_axis = glm::vec3(1.f, 0.3f, 0.8f);
        rot = glm::rotate(glm::mat4(1.f), glm::radians(67.f), rot_axis);
        Timer::get()->do_every(std::chrono::milliseconds(30), [&]() {
            rot_angle += Util::random(0.01f, 0.05f);
            rot = glm::rotate(glm::mat4(1.f), glm::radians(67.f), rot_axis);
            rot *= glm::rotate(glm::mat4(1.f), glm::radians(rot_angle), glm::vec3(0.f, 1.f, 0.f));
        });
    });
}

void SkyboxShader::pre_draw(SceneInfo &scene_info) {}
void SkyboxShader::post_draw() {}

void SkyboxShader::draw(Mesh *mesh, SceneInfo &scene_info, glm::mat4 to_world) {
    // Disable back face culling because we are in inside of cube
    glDisable(GL_CULL_FACE);

    // Strip translation from view matrix to make skybox appear infinitely large.
    glm::mat4 view = glm::mat4(glm::mat3(scene_info.camera.V));

    // Send view and projection matrices
    set_uni("view", view);
    set_uni("projection", scene_info.camera.P);
    set_uni("rot", rot);

    // Bind active texture.
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(get_uni("skybox"), 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, current_texture_id);

    cube_geometry->bind();
    cube_geometry->draw();
    glBindVertexArray(0);

    // Re-enable back face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}
