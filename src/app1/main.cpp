#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <chrono>
#include <random>

#include "../common/sharedInclude.h"
#include "../common/utils.h"
#include "../common/shaderLoadException.h"
#include "../common/effect.h"
#include "../common/texture.h"

#include "reel.h"
#include "frame.h"

class Scene
{
public:

	Scene() 
		: isLeftButtonPressed(false),
		cameraPosition(0, 0, 7),
		cameraRight(1, 0, 0),
		cameraUp(0, 1, 0),
		cameraLook(0, 0, -1),
		mt(std::random_device()())
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

		reelTexture.LoadFromFile("textures/reel.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
	
		CreateReelMesh(reelMesh, 12);
		
		for (int i = 0; i < 5; i++)
		{
			reels.push_back(std::make_unique<Reel>(reelTexture, reelEffect, reelMesh, 12));
			auto reel = reels[i].get();
			reel->model = glm::scale(glm::mat4(), glm::vec3(0.5f, 2.0f, 2.0f));
			
			reel->model = glm::translate(glm::mat4(), glm::vec3(i - 2.0f, 0, 0)) * reel->model;
		}

		Shader vertex2(GL_VERTEX_SHADER), fragment2(GL_FRAGMENT_SHADER);
		frameEffect.Attach(vertex2.CompileFile("shaders/frameVertex.glsl"))
			.Attach(fragment2.CompileFile("shaders/frameFragment.glsl")).Link();

		Shader vertex3(GL_VERTEX_SHADER), fragment3(GL_FRAGMENT_SHADER);
		glassEffect.Attach(vertex3.CompileFile("shaders/glassVertex.glsl"))
			.Attach(fragment3.CompileFile("shaders/glassFragment.glsl")).Link();
		
		CreateFrameMesh(frameMesh);
		CreateCube(glassMesh);
		
		frame = std::make_unique<Frame>(frameEffect, frameMesh, glassEffect, glassMesh);
		frame->model = frame->model * glm::translate(glm::mat4(), glm::vec3(-0.0f, -0.0f, 2.4f));

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


		for (auto &reel : reels)
		{
			reel->Render(view, projection);
		}

		frame->Render(view, projection, cameraPosition);

		glutSwapBuffers();
	}

	void Update(float deltaMs)
	{
		for (auto &reel : reels)
		{
			reel->Update(deltaMs);
		}

		reels[0]->Stop();
		for (size_t i = 1; i < reels.size(); i++)
		{
			if (reels[i - 1]->IsStop())
			{
				reels[i]->Stop();
			}
		}
	}

	void OnKeyPress(unsigned char key, int x, int y)
	{
		if (key == ' ')
		{
			for (size_t i = 0; i < reels.size(); i++)
			{
				float j = static_cast<float>(i);
				float mult = 1.0f;
				auto dist = std::uniform_real_distribution<float>((1) * mult, (1.2f) * mult);
				reels[i]->Roll(dist(mt));
			}
		}
	}

	void OnMouseAction(int button, int state, int x, int y)
	{
		mousePosition = glm::vec2(x, y);
		if (button == GLUT_LEFT_BUTTON)
		{
			isLeftButtonPressed = state == GLUT_DOWN;
		}
		else if (button == GLUT_RIGHT_BUTTON) 
		{
			isRightButtonPressed = state == GLUT_DOWN;
		}
		glutPostRedisplay();
	}
		
	void OnMouseMove(int isActive, int x, int y)
	{
		if (isActive)
		{
			if (isLeftButtonPressed)
			{
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
			else if (isRightButtonPressed)
			{
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
	Effect frameEffect;
	Effect glassEffect;
	Texture reelTexture;
	Mesh reelMesh;
	Mesh frameMesh;
	Mesh glassMesh;

	std::vector<std::unique_ptr<Reel>> reels;
	std::unique_ptr<Frame> frame;

	std::mt19937 mt;
};


Scene scene;
std::chrono::time_point<std::chrono::steady_clock> start;
const float MIN_UPDATE_PERIOD_MS = 1000.0f / 120;

void UpdateLoop()
{
	auto current = std::chrono::steady_clock::now();
	using float_ms = std::chrono::duration<float, std::milli>;
	float elapsed = std::chrono::duration_cast<float_ms>(current - start).count();
	if (elapsed > MIN_UPDATE_PERIOD_MS)
	{
		start = current;
		scene.Update(elapsed);
	}
	glutPostRedisplay();
}


int main(int argc, char *argv[])
{
	glutInitWarningFunc([](const char *fmt, va_list ap) 
	{
		std::cerr << "GLUT warning: " << VFormat(fmt, ap);
	});
	glutInitErrorFunc([](const char *fmt, va_list ap) 
	{
		std::cerr << "GLUT error: " << VFormat(fmt, ap);
	});

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(800, 800);

	glutCreateWindow("Lab 2 -- OpenGL");

	GLenum glewInitResult = glewInit();
	if (glewInitResult != GLEW_OK) 
	{
		std::cerr << "GLEW error: " << reinterpret_cast<const char *>(glewGetErrorString(glewInitResult));
		return 1;
	}

	try
	{
		scene.Init();
	}
	catch (ShaderLoadException &ex) 
	{
		std::string message = ex.ShaderPath().length() > 0
			? (ex.ShaderPath() + ": ") : "<in-memory shader>: ";
		std::cerr << message << ex.what();
		return 3;
	}
	catch (std::runtime_error &err)
	{
		std::cerr << err.what();
		return 4;
	}


	glutReshapeFunc([](int width, int height)
	{
		glViewport(0, 0, width, height);
		scene.UpdateProjection(width, height);
	});

	glutDisplayFunc([]() 
	{
		scene.Render();
	});
		
	glutKeyboardFunc([](unsigned char key, int x, int y) 
	{
		scene.OnKeyPress(key, x, y);
		UpdateLoop();
	});

	glutMouseFunc([](int button, int state, int x, int y) 
	{
		scene.OnMouseAction(button, state, x, glutGet(GLUT_WINDOW_HEIGHT) - y);
		UpdateLoop();
	});

	glutMotionFunc([](int x, int y)
	{
		scene.OnMouseMove(true, x, glutGet(GLUT_WINDOW_HEIGHT) - y);
		UpdateLoop();
	});

	glutPassiveMotionFunc([](int x, int y)
	{
		scene.OnMouseMove(false, x, glutGet(GLUT_WINDOW_HEIGHT) - y);
		UpdateLoop();
	});

	glutIdleFunc(UpdateLoop);
	
	start = std::chrono::steady_clock::now();

	glutMainLoop();
}