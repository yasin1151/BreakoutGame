#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Game.h"
#include "ResourceMgr.h"

using namespace std;


const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;
const char* TITLE_NAME = "2DGame";


// 全局回调函数
void OnFrameBufferSizeChange(GLFWwindow* pWindow, int nWidth, int nHeight);
void ProcessInput(GLFWwindow* pWindow);


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

	// 加载glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to Init GLAD" << endl;
		glfwTerminate();
		return -1;
	}

	cout << "Game Start !" << endl;
	Game oGame = Game(SCREEN_WIDTH, SCREEN_HEIGHT);
	oGame.Init();

	while(!glfwWindowShouldClose(pWindow))
	{
		// oGame.ProcessInput(0.016);
		ProcessInput(pWindow);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		oGame.Render();

		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}

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

void ProcessInput(GLFWwindow* pWindow)
{
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(pWindow, true);
	}
}