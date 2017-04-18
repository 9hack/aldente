#include "scene_model.h"

#include "util/util.h"

SceneModel::SceneModel(Scene *scene)
{
    this->scene = scene;
	model_mat = glm::mat4(1.0f);
}

SceneModel::~SceneModel() {}

void SceneModel::add_mesh(Mesh* m)
{
    meshes.push_back(m);
}

void SceneModel::draw()
{
    // Loop over meshes and their respective shader programs.
    for (Mesh* mesh : meshes)
    {
        mesh.shader->use();
        mesh.shader->set_VP(scene->camera->V, scene->camera->P);
        mesh.shader->send_cam_pos(scene->camera->cam_pos);
        mesh.shader->send_mesh_model(mesh.to_world);
        mesh.shader->set_material(mesh.material);

        if (mesh.no_culling)
            glDisable(GL_CULL_FACE);
        else {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }
        mesh.shader->draw(mesh.geometry, model_mat);
    }
}

void SceneModel::update()
{

}

void SceneModel::pass(Shader * s)
{
    for (Mesh* mesh : meshes)
    {
        if (mesh.geometry) {
            s->send_mesh_model(mesh.to_world);
            s->draw(mesh.geometry, model_mat);
        }
    }
}

void SceneModel::combine_meshes()
{
    // Stop if one or zero meshes.
    if (meshes.size() <= 1)
        return;

    Mesh* mega_mesh = meshes.at(0);
    Geometry * mega_geometry = new Geometry();
    // TODO: GARBAGE COLLECT THIS PIECE OF GEOMETRY.
    mega_mesh->geometry = mega_geometry;
    mega_mesh->to_world = glm::mat4(1.f);

    unsigned int index_offset = 0;
    for (Mesh* mesh : meshes)
    {
        for (glm::vec3 v : mesh.geometry->vertices)
            mega_geometry->vertices.push_back(glm::vec3(mesh.to_world * glm::vec4(v, 1.f)));
        for (glm::vec3 n : mesh.geometry->normals) {
            mega_geometry->normals.push_back(glm::normalize(glm::mat3(glm::transpose(glm::inverse(mesh.to_world))) * n));
            // mega_geometry->normals.push_back(glm::vec3(mesh.to_world * glm::vec4(n, 1.f))); Incorrect normals for trees.
        }
        for (unsigned int i : mesh->geometry->indices)
            mega_geometry->indices.push_back(i+index_offset);
        index_offset += (unsigned int) mesh->geometry->indices.size();
    }
    mega_geometry->populate_buffers();
    meshes.clear();
    add_mesh(mega_mesh);
}

void SceneModel::setScene(Scene* scene) {
	this->scene = scene;
}
