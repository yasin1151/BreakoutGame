#ifndef __POST_PROCESSING_H__
#define __POST_PROCESSING_H__

// 后处理特效

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "texture.h"
#include "SpriteRenderer.h"
#include "Shader.h"

class PostProcessor
{
public:
	Shader PostProcessingShader;

	Texture2D Texture;
	GLuint Width, Height;

	GLboolean Confuse, Chaos, Shake;

	PostProcessor(Shader shader, GLuint width, GLuint height);

	void BeginRender();

	void EndRender();

	void Render(GLfloat time);

private:
	GLuint MSFBO, FBO;
	GLuint RBO;
	GLuint VAO;

	void initRenderData();

	void initProcessorData();

};


#endif