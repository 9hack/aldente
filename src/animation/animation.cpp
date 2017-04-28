#include "animation.h"
#include <iostream>

Animation::Animation(const aiScene *scene, const aiAnimation *anim)
    : scene(scene), anim(anim) {
    std::cerr << "Loading Anim: " << anim->mName.data << std::endl;
}

aiNode *Animation::get_root() {
    return scene->mRootNode;
}

const aiAnimation *Animation::get_anim() {    
    //std::cerr << "Get Anim: " << anim->mName.data << std::endl;
    return anim;
}