#include <iostream>
#include <algorithm>
#include <memory>

#include "../common/sharedInclude.h"
#include "../common/utils.h"
#include "../common/shaderLoadException.h"
#include "../common/effect.h"
#include "../common/texture.h"

#include "reel.h"

class Scene
{
public:

	Scene() 
		: isLeftButtonPressed(false),
		cameraPosition(0, 0, 5),
		cameraRight(1, 0, 0),
		cameraUp(0, 1, 0),
		cameraLook(0, 0, -1)
	{
		UpdateCamera();
	}

	void UpdateCamera()
	{
		auto position = cameraPosition;
		//position.y = std::max(0.0f, position.y);
		view = glm::lookAt(position, position + cameraLook, cameraUp);
	}

	void Init()
	{
		glClearColor(0, 0, 0, 1);

		Shader vertex(GL_VERTEX_SHADER), fragment(GL_FRAGMENT_SHADER);
		reelEffect.Attach(vertex.CompileFile("shaders/reelVertex.glsl"))
			.Attach(fragment.CompileFile("shaders/reelFragment.glsl")).Link();

		reelTexture.LoadFromFile("textures/reel.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE);
	
		CreateReelMesh(reelMesh, 12);

		reel = std::make_unique<Reel>(reelTexture, reelEffect, reelMesh);
		reel->model = glm::scale(glm::mat4(), glm::vec3(2, 2, 2));

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glEnableClientState(GL_VERTEX_ATTRIB_ARRAY_POINTER);
	}

	void UpdateProjection(int width, int height)
	{
		float aspectRatio = 1.0f * width / height;
		projection = glm::perspective(45.0f, aspectRatio, 1.0f, 500.0f);
	}

	void Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		reel->Render(view, projection);


		glutSwapBuffers();
	}

	void OnKeyPress(unsigned char key, int x, int y)
	{

	}

	void OnMouseAction(int button, int state, int x, int y)
	{
		mousePosition = glm::vec2(x, y);
		if (button == GLUT_LEFT_BUTTON) {
			isLeftButtonPressed = state == GLUT_DOWN;
		}
		else if (button == GLUT_RIGHT_BUTTON) {
			isRightButtonPressed = state == GLUT_DOWN;
		}
		glutPostRedisplay();
	}
		
	void OnMouseMove(int isActive, int x, int y)
	{
		if (isActive) {
			if (isLeftButtonPressed) {
				auto newMousePosition = glm::vec2(x, y);
				auto diff = newMousePosition - mousePosition;
				mousePosition = newMousePosition;

				glm::mat4 rotation = glm::rotate(glm::mat4(), diff.y * .005f, cameraRight);
				cameraUp = (rotation * glm::vec4(cameraUp, 1)).xyz;
				cameraLook = (rotation * glm::vec4(cameraLook, 1)).xyz;
				cameraPosition = (rotation * glm::vec4(cameraPosition, 1)).xyz;

				rotation = glm::rotate(glm::mat4(), -diff.x * .005f, glm::vec3(0, 1, 0));
				cameraRight = (rotation * glm::vec4(cameraRight, 1)).xyz;
				cameraUp = (rotation * glm::vec4(cameraUp, 1)).xyz;
				cameraLook = (rotation * glm::vec4(cameraLook, 1)).xyz;
				cameraPosition = (rotation * glm::vec4(cameraPosition, 1)).xyz;

				UpdateCamera();
				glutPostRedisplay();
			}
			else if (isRightButtonPressed) {
				mousePosition = glm::vec2(x, y);
				glutPostRedisplay();
			}
		}
	}

	
private:
	
	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 cameraPosition;
	glm::vec3 cameraRight, cameraUp, cameraLook;

	bool isLeftButtonPressed;
	bool isRightButtonPressed;
	glm::vec2 mousePosition;

	Effect reelEffect;
	Texture reelTexture;
	Mesh reelMesh;
	std::unique_ptr<Reel> reel;
};


Scene scene;


int main(int argc, char *argv[])
{
	glutInitWarningFunc([](const char *fmt, va_list ap) {
		std::cerr << "GLUT warning: " << VFormat(fmt, ap);
	});
	glutInitErrorFunc([](const char *fmt, va_list ap) {
		std::cerr << "GLUT error: " << VFormat(fmt, ap);
	});

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(800, 800);

	glutCreateWindow("Lab 2 -- OpenGL");

	GLenum glewInitResult = glewInit();
	if (glewInitResult != GLEW_OK) {
		std::cerr << "GLEW error: " << reinterpret_cast<const char *>(glewGetErrorString(glewInitResult));
		return 1;
	}

	try {
		scene.Init();
	}
	catch (ShaderLoadException &ex) {
		std::string message = ex.ShaderPath().length() > 0
			? (ex.ShaderPath() + ": ") : "<in-memory shader>: ";
		std::cerr << message << ex.what();
		return 3;
	}
	catch (std::runtime_error &err) {
		std::cerr << err.what();
		return 4;
	}


	glutReshapeFunc([](int width, int height) {
		glViewport(0, 0, width, height);
		scene.UpdateProjection(width, height);
	});

	glutDisplayFunc([]() {
		scene.Render();
	});

	glutKeyboardFunc([](unsigned char key, int x, int y) {
		scene.OnKeyPress(key, x, y);
	});

	glutMouseFunc([](int button, int state, int x, int y) {
		scene.OnMouseAction(button, state, x, glutGet(GLUT_WINDOW_HEIGHT) - y);
	});

	glutMotionFunc([](int x, int y) {
		scene.OnMouseMove(true, x, glutGet(GLUT_WINDOW_HEIGHT) - y);
	});

	glutPassiveMotionFunc([](int x, int y) {
		scene.OnMouseMove(false, x, glutGet(GLUT_WINDOW_HEIGHT) - y);
	});

	glutMainLoop();
}