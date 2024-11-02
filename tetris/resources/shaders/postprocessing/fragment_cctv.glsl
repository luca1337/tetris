#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;
uniform float time;

const float noiseAmount = 0.4;  // Amount of noise
const float scanlineIntensity = 0.1;  // Intensity of scan lines
const float vignetteIntensity = 0.5;  // Intensity of vignette

// Function to generate random noise
float random(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    vec2 uv = TexCoords;

    // Apply grayscale
    vec3 color = texture(screenTexture, uv).rgb;
    float gray = dot(color, vec3(0.299, 0.587, 0.114));
    // color = vec3(gray);

    // Apply noise
    float noise = random(uv + time) * noiseAmount;
    color += noise;

    // Apply scan lines
    float scanline = sin(uv.y * 100.0) * scanlineIntensity;
    color += vec3(scanline);

    // Apply vignette
    float dist = distance(uv, vec2(0.5, 0.5));
    color *= smoothstep(0.8, 0.5, dist * vignetteIntensity);

    FragColor = vec4(color, 1.0);
}