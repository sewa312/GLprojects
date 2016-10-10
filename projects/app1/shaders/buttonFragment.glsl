#version 330

uniform sampler2D TextureUnit0;
uniform float Time;

in vec2 FragTexCoord;

layout(location = 0) out vec4 FragmentColor;

void main()
{
    vec4 color1 = texture(TextureUnit0, FragTexCoord * vec2(1, 0.5f));
    vec4 color2 = texture(TextureUnit0, 
      FragTexCoord * vec2(1, 0.5f) + vec2(0, 0.5f));
 
    float delta = 1000.0f;
    float simpleTime = abs(Time - round(Time / delta) * delta) / delta;
    vec4 color = sqrt(mix(color1 * color1, color2 * color2, simpleTime * 2));
    FragmentColor = color;
}