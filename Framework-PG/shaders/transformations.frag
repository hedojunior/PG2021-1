#version 430 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;
uniform float offsetX;
uniform float offsetY;

void main()
{
    color = texture(ourTexture1, vec2(TexCoord.x + offsetX, TexCoord.y + offsetY));
}