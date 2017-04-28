#pragma once

#include "shader.h"

class BasicAnimShader :
        public Shader {
public:
    BasicAnimShader(GLuint shader_id);

    void set_material(Material *m);

    void draw(Geometry *g, glm::mat4 to_world);
};

