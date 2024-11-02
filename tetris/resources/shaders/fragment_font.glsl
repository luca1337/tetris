#version 330 core

in vec2 TexCoord;

uniform sampler2D textTexture;

out vec4 FragColor;

void main() 
{
    FragColor = texture(textTexture, TexCoord);
}
