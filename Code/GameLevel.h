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

	// 从文件中加载关卡
	void Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);

	// 渲染关卡
	void Draw(SpriteRenderer& renderer);

	// 检查一个关卡是否完成
	GLboolean IsCompleted();

private:

	// 由砖块数据初始化关卡
	void init(TileVector tileData, GLuint levelWidth, GLuint levelHeight);

};


#endif
