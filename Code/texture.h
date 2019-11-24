#ifndef __Texture_H__
#define __Texture_H__

#include <iostream>
#include <GLFW/glfw3.h>

class Texture2D
{
public:

	// 纹理id
	GLuint ID;

	// 纹理大小
	GLuint Width, Height;
	
	// 纹理格式
	GLuint Internal_Format;	// 纹理在内存中的存储格式
	GLuint Image_Format;	// 纹理源数据格式

	// 纹理参数
	GLuint Wrap_S;	// 	s轴的纹理环绕方式
	GLuint Wrap_T;	//	t轴的纹理环绕方式
	GLuint Filter_Min;	// 缩小时的纹理过滤方式
	GLuint Filter_Max;	// 放大时的纹理过滤方式

	Texture2D();

	void Generate(GLuint width, GLuint height, unsigned char* data);

	void Bind() const;
};


#endif
