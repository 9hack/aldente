#version 330 core

out vec4 color;

in vec2 tex_coords;

uniform sampler2D depth_map;

void main()
{
    float depth_value = texture(depth_map, tex_coords).r;
    color = vec4(vec3(depth_value), 1.0); // orthographic
}