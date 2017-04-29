#pragma once

#include "glm/glm.hpp"
#include <assimp/scene.h>

/*
    Uses assimp's node structure to save animation
*/
class Animation {
public:            
    Animation(const aiScene *scene, const aiAnimation *anim);
    aiNode *get_root();
    const aiAnimation *get_anim();    

private:   
    const aiScene *scene;
    const aiAnimation *anim;    
};