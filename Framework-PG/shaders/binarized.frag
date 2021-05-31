#version 430 core
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;

uniform float binarizationFactor;

void main()
{
    color = texture(ourTexture1, TexCoord);

    color = vec4(color.r > binarizationFactor ? 255.0 : 0.0, 
                 color.g > binarizationFactor ? 255.0 : 0.0, 
                 color.b > binarizationFactor ? 255.0 : 0.0, 1.0);
}