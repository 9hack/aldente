#include "animation.h"
#include <iostream>

Animation::Animation(const aiScene *scene, const aiAnimation *anim)
    : scene(scene), anim(anim) {    
}

aiNode *Animation::get_root() {
    return scene->mRootNode;
}

const aiAnimation *Animation::get_anim() {        
    return anim;
}