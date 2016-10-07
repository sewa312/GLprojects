#version 330

uniform mat4 Model;
uniform mat4 View;

in vec3 FragNormal_modelspace;
in vec3 Lightdirection_cameraspace;

out vec3 color;

void main()
{
    mat3 MV = mat3(Model * View);
	vec3 Normal_cameraspace = MV * normalize(FragNormal_modelspace);
	float cosTheta = clamp(dot(Normal_cameraspace, Lightdirection_cameraspace), 0, 1);
	float ambient = 0.5f;
	color = vec3(183.0f / 255.0f, 63.0f/ 255.0f, 14.0f / 255.0f) * vec3(min(1, cosTheta + ambient));
}
