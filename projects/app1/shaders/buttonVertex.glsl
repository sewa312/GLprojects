#version 330

uniform vec2 Viewport;

layout(location = 0) in vec2 Position_viewportspace;
layout(location = 3) in vec2 TexCoord0;

out vec2 FragTexCoord;

void main()
{
	vec2 Position_screenspace = 
		(Position_viewportspace * 2 / Viewport - vec2(1, 1));

	gl_Position = vec4(Position_screenspace, 0, 1);
	FragTexCoord = TexCoord0;
}