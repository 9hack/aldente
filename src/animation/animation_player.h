#pragma once

#include "model\model.h"

class AnimationPlayer {
public:
    AnimationPlayer();
    ~AnimationPlayer();

    void play(float time_in_secs, Animation *animation, Model *model);    

private:    

    void process_animation(float anim_time, const aiAnimation *anim, Model *model, const aiNode *node, glm::mat4 parent_mat);

    const aiNodeAnim *find_node_anim(const aiAnimation *anim, const std::string node_name);
    glm::mat4 convert_ai_matrix(aiMatrix4x4 ai_mat);

    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
};