#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 baseColor;
uniform bool hasTexture;

void main()
{
	vec4 sampled = vec4(1.0);

	if (hasTexture)
	{
		sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
	}
    color = vec4(baseColor, 1.0) * sampled;
}
