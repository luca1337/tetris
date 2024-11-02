#version 330 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 uvs;
layout (location = 2) in mat4 model;
layout (location = 6) in vec4 instanceColor;  // Location 6 for color because model is 4 vec4's

uniform mat4 view;
uniform mat4 projection;

out vec4 outColor;
out vec2 TexCoord;

void main()
{
    vec4 worldPosition = model * vec4(vertex, 0.0, 1.0);
    gl_Position = projection * view * worldPosition;

    outColor = instanceColor;
    TexCoord = uvs;
}