#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coords;

out vec2 frag_tex_coords;

void main()
{
    gl_Position = vec4(position, 1.0f);
    frag_tex_coords = tex_coords;
}