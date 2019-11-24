#ifndef __Texture_H__
#define __Texture_H__

#include <iostream>
#include <GLFW/glfw3.h>

class Texture2D
{
public:

	// ����id
	GLuint ID;

	// �����С
	GLuint Width, Height;
	
	// �����ʽ
	GLuint Internal_Format;	// �������ڴ��еĴ洢��ʽ
	GLuint Image_Format;	// ����Դ���ݸ�ʽ

	// �������
	GLuint Wrap_S;	// 	s��������Ʒ�ʽ
	GLuint Wrap_T;	//	t��������Ʒ�ʽ
	GLuint Filter_Min;	// ��Сʱ��������˷�ʽ
	GLuint Filter_Max;	// �Ŵ�ʱ��������˷�ʽ

	Texture2D();

	void Generate(GLuint width, GLuint height, unsigned char* data);

	void Bind() const;
};


#endif
