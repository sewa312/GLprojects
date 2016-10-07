#version 330

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 LightPosition_worldspace;

layout(location = 0) in vec3 Position_modelspace;
layout(location = 2) in vec3 Normal_modelspace;

out vec3 FragNormal_modelspace;
out vec3 Lightdirection_cameraspace;

void main()
{
    vec3 position_worldspace = vec3(Model * vec4(Position_modelspace, 1));
	gl_Position = Projection * View * vec4(position_worldspace, 1);
	FragNormal_modelspace = Normal_modelspace;

	vec3 Lightdirection_worldspace = normalize(LightPosition_worldspace - position_worldspace);
	Lightdirection_cameraspace = normalize(mat3(View) * inverse(transpose(mat3(Model))) * Lightdirection_worldspace);
}
