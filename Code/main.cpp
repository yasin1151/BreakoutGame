#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Game.h"
#include "ResourceMgr.h"

using namespace std;


const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;
const char* TITLE_NAME = "2DGame";

Game BreakoutGame = Game(SCREEN_WIDTH, SCREEN_HEIGHT);

// 全局回调函数
void OnFrameBufferSizeChange(GLFWwindow* pWindow, int nWidth, int nHeight);
void ProcessInput(GLFWwindow* pWindow);
void OnKeyCallBack(GLFWwindow* pWindow, int key, int scancode, int action, int mode);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* pWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE_NAME, nullptr, nullptr);
	if (!pWindow)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(pWindow);
	glfwSetFramebufferSizeCallback(pWindow, OnFrameBufferSizeChange);
	glfwSetKeyCallback(pWindow, OnKeyCallBack);

	// 加载glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to Init GLAD" << endl;
		glfwTerminate();
		return -1;
	}

	cout << "Game Start !" << endl;

	BreakoutGame.Init();

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	BreakoutGame.State = GAME_ACTIVE;


	while(!glfwWindowShouldClose(pWindow))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		BreakoutGame.ProcessInput(deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		BreakoutGame.Render();

		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}

	ResourceMgr::Clear();

	glfwTerminate();
	return 0;
}


void OnFrameBufferSizeChange(GLFWwindow* pWindow, int nWidth, int nHeight)
{
	if (pWindow)
	{
		glViewport(0, 0, nWidth, nHeight);
	}
}

void OnKeyCallBack(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(pWindow, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			BreakoutGame.Keys[key] = GL_TRUE;
		}
		else if (action == GLFW_RELEASE)
		{
			BreakoutGame.Keys[key] = GL_FALSE;
			BreakoutGame.KeysProcessed[key] = GL_FALSE;
		}
	}

}