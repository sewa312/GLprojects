#version 330

uniform sampler2D TextureUnit0;

in vec2 FragTexCoord;

layout(location = 0) out vec4 FragmentColor;

void main()
{
	FragmentColor = texture2D(TextureUnit0, FragTexCoord);
}