#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;
uniform float time;
uniform float glitchStartTime;
uniform float glitchDuration;

// Shake parameters
uniform float shake_power = 0.02;
uniform float shake_rate = 0.5; // Use the range [0.0, 1.0]
uniform float shake_speed = 5.0;
uniform float shake_block_size = 30.5;

uniform float chroma_aberration_strength = 0.02;

float random(float seed)
{
    return fract(sin(dot(vec2(seed, seed), vec2(3525.46, -54.3415))) * 43758.5453);
}

void main()
{
    vec2 uv = TexCoords;
    vec3 color = texture(screenTexture, uv).rgb;

    if (time >= glitchStartTime && time < glitchStartTime + glitchDuration) 
    {
        // Apply random distortion
        float enable_shift = float(random(floor(time * shake_speed)) < shake_rate);
        uv.x += (random((floor(TexCoords.y * shake_block_size) / shake_block_size) + time) - 0.5) * shake_power * enable_shift;

        // Apply color shift with chromatic aberration effect
        vec3 shiftedColor;
        shiftedColor.r = texture(screenTexture, uv + vec2(chroma_aberration_strength, 0.0)).r;
        shiftedColor.g = texture(screenTexture, uv).g;
        shiftedColor.b = texture(screenTexture, uv - vec2(chroma_aberration_strength, 0.0)).b;

        color = mix(color, shiftedColor, 0.5);
    }

    FragColor = vec4(color, 1.0);
}
