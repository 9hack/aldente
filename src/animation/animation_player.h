#pragma once

#include "model/model.h"

/*
    Used for actually processing the animations and adjusting the bone matrices to actually
    pose the model
*/
class AnimationPlayer {
public:
    void play(Model *model, std::string anim_name);

    void set_speed(float speed);

private:
    float speed;

    void process_animation(float anim_time, const aiAnimation *anim, Model *model, const aiNode *node, glm::mat4 parent_mat);

    const aiNodeAnim *find_node_anim(const aiAnimation *anim, const std::string node_name);
    glm::mat4 convert_ai_matrix(aiMatrix4x4 ai_mat);

    void calc_interpolated_scaling(aiVector3D& out, float anim_time, const aiNodeAnim* node_anim);
    void calc_interpolated_rotation(aiQuaternion& out, float anim_time, const aiNodeAnim* node_anim);
    void calc_interpolated_position(aiVector3D& out, float anim_time, const aiNodeAnim* node_anim);
    unsigned int find_scaling(float anim_time, const aiNodeAnim* node_anim);
    unsigned int find_rotation(float anim_time, const aiNodeAnim* node_anim);
    unsigned int find_position(float anim_time, const aiNodeAnim* node_anim);
};