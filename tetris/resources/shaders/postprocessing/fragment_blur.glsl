#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;

vec3 applyStrongBlur(sampler2D image, vec2 uv)
{
    vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
    vec3 result = vec3(0.0);
    float blurSize = 10.0; // Increase for stronger blur
    float totalWeight = 0.0;
    
    for (float x = -blurSize; x <= blurSize; x += 1.0)
    {
        for (float y = -blurSize; y <= blurSize; y += 1.0)
        {
            float weight = exp(-(x * x + y * y) / (2.0 * blurSize));
            result += texture(image, uv + vec2(x * tex_offset.x, y * tex_offset.y)).rgb * weight;
            totalWeight += weight;
        }
    }

    return result / totalWeight;
}
void main()
{
    vec2 uv = TexCoords;
    vec3 color = texture(screenTexture, uv).rgb;

    color = applyStrongBlur(screenTexture, uv);
    color = mix(color, vec3(0.0, 0.0, 0.0), 0.8);

    FragColor = vec4(color, 1.0);
}