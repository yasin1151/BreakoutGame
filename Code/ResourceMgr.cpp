#include "ResourceMgr.h"

std::map<std::string, Shader> ResourceMgr::ShaderMap;
std::map<std::string, Texture2D> ResourceMgr::TextureMap;

Shader ResourceMgr::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile,
	std::string name)
{
	if (ShaderMap.count(name))
	{
		return ShaderMap[name];
	}

	ShaderMap[name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return ShaderMap[name];
}

Shader& ResourceMgr::GetShader(std::string name)
{
	return ShaderMap[name];
}

Texture2D ResourceMgr::LoadTexture(const char* file, GLboolean alpha, std::string name)
{
	if (TextureMap.count(name))
	{
		return TextureMap[name];
	}
	TextureMap[name] = LoadTextureFromFile(file, alpha);
	return TextureMap[name];
}

Texture2D& ResourceMgr::GetTexture(std::string name)
{
	return TextureMap[name];
}

void ResourceMgr::Clear()
{
	for (auto iter : ShaderMap)
	{
		glDeleteProgram(iter.second.GetShaderProgramID());
	}

	for (auto iter : TextureMap)
	{
		glDeleteTextures(1, &iter.second.ID);
	}

}

ResourceMgr::ResourceMgr()
{
}

Shader ResourceMgr::LoadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
	Shader oShader = Shader(vShaderFile, fShaderFile);
	return oShader;
}

Texture2D ResourceMgr::LoadTextureFromFile(const char* file, GLboolean alpha)
{
	// Create Texture object
	Texture2D texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	// Load image
	int width, height;
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	if (!image)
	{
		std::cout << "Filed Load Texture" << std::endl;
	}
	// Now generate texture
	texture.Generate(width, height, image);
	// And finally free image data
	SOIL_free_image_data(image);
	return texture;
}
