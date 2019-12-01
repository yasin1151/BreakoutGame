#include "GameLevel.h"



GameLevel::GameLevel()
{

}

void GameLevel::Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight)
{
	if (!file || !levelWidth || !levelWidth)
	{
		std::cout << "GameLevel::Load Failed :" << file << levelWidth << levelHeight << std::endl;
		return;
	}
	// 先清理
	this->Bricks.clear();

	// 从文件中加载
	GLuint tileCode;

	std::string line;
	std::ifstream fstream(file);

	TileVector tileData;

	if (!fstream)
	{
		std::cout << "GameLevel::Load Failed, file open failed" << file << std::endl;
	}

	while (std::getline(fstream, line))	// 读取关卡文件每一行
	{
		
		std::stringstream sstream(line);
		UINTVector row;
		while(sstream >> tileCode)
		{
			row.push_back(tileCode);
		}
		tileData.push_back(row);
	}

	if (!tileData.empty())
	{
		this->init(tileData, levelWidth, levelHeight);
	}
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
	for (auto tile : this->Bricks)
	{
		if (!tile.Destroyed)
		{
			tile.Draw(renderer);
		}
	}
}

GLboolean GameLevel::IsCompleted()
{
	for (auto tile : this->Bricks)
	{
		if (!tile.IsSoild && !tile.Destroyed)
		{
			return GL_FALSE;
		}
	}
	return GL_TRUE;
}

void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
	if (tileData.empty())
	{
		std::cout << "GameLevel::init Failed, tileData Is Empty" << std::endl;
		return;
	}


	// 计算每个维度的大小
	GLuint height = tileData.size();
	GLuint width = tileData[0].size();

	GLfloat unit_width = levelWidth / static_cast<GLfloat>(width);
	GLfloat unit_height = levelHeight / height;

	// 基于tileData初始化关卡

	for (GLuint y = 0; y < height; ++y)
	{
		for (GLuint x = 0; x < width; ++x)
		{
			GLuint iType = tileData[y][x];
			const char* szTextureName = "block";
			glm::vec2 pos(unit_width * x, unit_height * y);
			glm::vec2 size(unit_width, unit_height);
			glm::vec3 color(1.0f);

			switch(iType)
			{
			case 1:
				szTextureName = "block_solid";
				color = glm::vec3(0.8f, 0.8f, 0.7f);
				break;
			case 2:
				color = glm::vec3(0.2f, 0.6f, 1.0f);
				break;
			case 3:
				color = glm::vec3(0.0f, 0.7f, 0.0f);
				break;
			case 4:
				color = glm::vec3(0.8f, 0.8f, 0.4f);
				break;
			case 5:
				color = glm::vec3(1.0f, 0.5f, 0.0f);
				break;
			default: 
				continue;
			}

			GameObject obj(pos, size, ResourceMgr::GetTexture(szTextureName), color);
			this->Bricks.push_back(obj);
		}
	}

}
