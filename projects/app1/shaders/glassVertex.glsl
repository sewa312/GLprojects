#version 330

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

layout(location = 0) in vec3 Position_modelspace;
layout(location = 3) in vec2 TexCoord0;

out vec2 FragTexCoord;

void main()
{
    vec3 position_worldspace = vec3(Model * vec4(Position_modelspace, 1));
    gl_Position = Projection * View * vec4(position_worldspace, 1);
	FragTexCoord = TexCoord0;
}
