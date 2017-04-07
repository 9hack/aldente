#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view_projection;
uniform mat4 mesh_model;

void main()
{
    gl_Position = view_projection * model * mesh_model * vec4(position, 1.0f);
}
