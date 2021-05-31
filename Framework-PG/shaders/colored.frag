#version 430 core
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;
uniform vec3 rgbModifier;

void main()
{
    color = texture(ourTexture1, TexCoord);

    color = vec4(color.r + rgbModifier.r, color.g + rgbModifier.g, color.b + rgbModifier.b, 1.0);
}
