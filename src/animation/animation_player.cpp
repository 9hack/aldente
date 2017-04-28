#include "animation_player.h"

AnimationPlayer::AnimationPlayer() {

}

AnimationPlayer::~AnimationPlayer() {

}

void AnimationPlayer::play(float time_in_secs, Animation *animation, Model *model) {
    float ticks_per_sec = (float)(animation->get_anim()->mTicksPerSecond);
    float time_in_ticks = time_in_secs * ticks_per_sec;
    float animation_time = fmod(time_in_ticks, (float)animation->get_anim()->mDuration);

    // std::cerr << "Playing Animation at time : " << animation_time << std::endl;

    process_animation(animation_time, animation->get_anim(), model, animation->get_root(), glm::mat4(1.0f));
}

void AnimationPlayer::process_animation(float anim_time, const aiAnimation *anim, Model *model, const aiNode *node, glm::mat4 parent_mat) {
    std::string node_name(node->mName.data);    

    glm::mat4 node_trans = convert_ai_matrix(node->mTransformation);

    const aiNodeAnim *node_anim = find_node_anim(anim, node_name);

    if (node_anim) { 
        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D Scaling;
        CalcInterpolatedScaling(Scaling, anim_time, node_anim);
        glm::mat4 ScalingM = glm::scale(glm::mat4(1.0f), glm::vec3(Scaling.x, Scaling.y, Scaling.z));

        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion RotationQ;
        CalcInterpolatedRotation(RotationQ, anim_time, node_anim);
        glm::mat4 RotationM = glm::mat4(convert_ai_matrix(aiMatrix4x4(RotationQ.GetMatrix())));

        // Interpolate translation and generate translation transformation matrix
        aiVector3D Translation;
        CalcInterpolatedPosition(Translation, anim_time, node_anim);
        glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), glm::vec3(Translation.x, Translation.y, Translation.z));

        // Combine the above transformations
        node_trans = TranslationM * RotationM * ScalingM;
    }

    glm::mat4 global_trans = parent_mat * node_trans;

    if (model->bone_mapping.find(node_name) != model->bone_mapping.end()) {
        unsigned int bone_index = model->bone_mapping[node_name];
        model->bones_final[bone_index] = model->global_inv_trans * global_trans * model->bone_offsets[bone_index];  //model->bone_offsets[bone_index];
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        process_animation(anim_time, anim, model, node->mChildren[i], global_trans);
    }
}

const aiNodeAnim* AnimationPlayer::find_node_anim(const aiAnimation *anim, const std::string node_name) {

    // std::cerr << "Finding Node Animtion " << anim->mNumChannels << std::endl;

    for (unsigned int i = 0; i < anim->mNumChannels; i++) {
        const aiNodeAnim* pNodeAnim = anim->mChannels[i];

        if (std::string(pNodeAnim->mNodeName.data) == node_name) {
            return pNodeAnim;
        }
    }

    return NULL;
}

glm::mat4 AnimationPlayer::convert_ai_matrix(aiMatrix4x4 ai_mat) {
    return{ ai_mat.a1, ai_mat.b1, ai_mat.c1, ai_mat.d1,
        ai_mat.a2, ai_mat.b2, ai_mat.c2, ai_mat.d2,
        ai_mat.a3, ai_mat.b3, ai_mat.c3, ai_mat.d3,
        ai_mat.a4, ai_mat.b4, ai_mat.c4, ai_mat.d4 };
}




unsigned int AnimationPlayer::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim) {
    for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }

    assert(0);
    return 0;
}


unsigned int AnimationPlayer::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim) {
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
        if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }

    assert(0);
    return 0;
}


unsigned int AnimationPlayer::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim) {
    assert(pNodeAnim->mNumScalingKeys > 0);

    for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
        if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }

    assert(0);
    return 0;
}


void AnimationPlayer::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim) {
    if (pNodeAnim->mNumPositionKeys == 1) {
        Out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }

    unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
    unsigned int NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
    float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}


void AnimationPlayer::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim) {
    // we need at least two values to interpolate...
    if (pNodeAnim->mNumRotationKeys == 1) {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }

    unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
    unsigned int NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out = Out.Normalize();
}


void AnimationPlayer::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim) {
    if (pNodeAnim->mNumScalingKeys == 1) {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
    unsigned int NextScalingIndex = (ScalingIndex + 1);
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}
