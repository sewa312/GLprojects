#version 330

in vec2 FragTexCoord;

out vec4 color;

void main()
{
    float input = (FragTexCoord.x + FragTexCoord.y ) * 8.0f;
    float noise = input - floor(input / 0.17f) * 0.17f;
    color = vec4(100.0f / 255.0f + noise, 149.0f/ 255.0f + noise, 237.0f / 255.0f + noise, 0.2f);
}
