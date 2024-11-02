#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;

in vec4 outColor;

void main()
{
    vec4 texColor = texture(ourTexture, TexCoord);
    FragColor = vec4(texColor.rgb, 1.0);
}
