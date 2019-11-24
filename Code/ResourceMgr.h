#ifndef __ResourceMgr_H__
#define __ResourceMgr_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "texture.h"
#include "Shader.h"

#include <map>
#include <string>
#include <SOIL.h>

#include <iostream>
#include <sstream>
#include <fstream>



// 单例资源管理器
class ResourceMgr
{
public:
	static std::map<std::string, Shader> ShaderMap;
	static std::map<std::string, Texture2D> TextureMap;

	static Shader LoadShader(const char* vsShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
	static Shader& GetShader(std::string name);


	static Texture2D LoadTexture(const char* file, GLboolean alpha, std::string name);
	static Texture2D& GetTexture(std::string name);

	static void Clear();

private:
	ResourceMgr();

	static Shader LoadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile);
	static Texture2D LoadTextureFromFile(const char* file, GLboolean alpha);

};


#endif
