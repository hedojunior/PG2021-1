#version 430 core
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;

void main()
{
    color = texture(ourTexture1, TexCoord);

    float colorAverage = color.r * 0.2125 + color.g * 0.7154 + color.b * 0.0721;

    color = vec4(colorAverage, colorAverage, colorAverage, 1.0);
}
