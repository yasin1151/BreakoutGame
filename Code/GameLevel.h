#ifndef __GameLevel_H__
#define __GameLevel_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "ResourceMgr.h"


class GameLevel
{
public:
	typedef std::vector<GLuint> UINTVector;
	typedef std::vector<UINTVector> TileVector;

	std::vector<GameObject> Bricks;

	GameLevel();

	// ���ļ��м��عؿ�
	void Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);

	// ��Ⱦ�ؿ�
	void Draw(SpriteRenderer& renderer);

	// ���һ���ؿ��Ƿ����
	GLboolean IsCompleted();

private:

	// ��ש�����ݳ�ʼ���ؿ�
	void init(TileVector tileData, GLuint levelWidth, GLuint levelHeight);

};


#endif
