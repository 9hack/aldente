#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

out vec3 frag_pos;
out vec3 frag_normal;
out vec4 frag_pos_light;
out vec2 frag_tex_coord;

const int MAX_BONES = 100;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light_matrix;
uniform mat4 mesh_model;
uniform mat4 bones[MAX_BONES];

void main()
{
	mat4 BoneTransform = bones[BoneIDs[0]] * Weights[0];
    BoneTransform += bones[BoneIDs[1]] * Weights[1];
    BoneTransform += bones[BoneIDs[2]] * Weights[2];
    BoneTransform += bones[BoneIDs[3]] * Weights[3];	

	vec4 new_pos = BoneTransform * vec4(position, 1.0);
	vec4 new_norm = BoneTransform * vec4(normal, 0.0);
    gl_Position = projection * view * model * mesh_model * new_pos;
    frag_pos = vec3(model * mesh_model * vec4(position, 1.0f));
    frag_normal = mat3(transpose(inverse(model * mesh_model))) * vec3(new_norm);
	frag_pos_light = light_matrix * vec4(frag_pos, 1.0);
	frag_tex_coord = vec2(tex_coord.x, 1.0 - tex_coord.y); //y-axis usually requires inverting
}
