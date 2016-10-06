#version 330

uniform sampler2D TextureUnit0;
uniform float Velocity;

in vec2 FragTexCoord;

layout(location = 0) out vec4 FragmentColor;

void main()
{
	vec4 colorSum = vec4(0, 0, 0, 0);
	for (int i = -5; i <= 5; i++) 
	{
		float shift = (float(i) / 5.0f) * clamp(Velocity * 0.01f, -0.3f, 0.3f);
		vec4 color = texture(TextureUnit0, FragTexCoord + vec2(0, shift));
		colorSum += color * color;
	}
	FragmentColor = sqrt(colorSum / 11.0f);
}