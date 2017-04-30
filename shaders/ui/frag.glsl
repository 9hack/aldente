#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 baseColor;
uniform bool hasTexture;
uniform float alpha;

void main()
{
    if (hasTexture)
        color = texture(text, TexCoords);
    else
        color = vec4(baseColor, alpha);
}