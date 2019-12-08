#ifndef __Shader_H__
#define __Shader_H__
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	typedef unsigned int UINT;
	const static UINT SHADER_LOG_BUF_SIZE = 1024;

	Shader();

	Shader(const std::string& szVertexPath, const std::string& szFragmentPath);

	bool InitWithFile(const std::string& szVertexPath, const std::string& szFragmentPath);

	bool InitWithCode(const std::string& szVertexCode, const std::string& szFragmentCode);

	void UseShader() const;

	[[nodiscard]] UINT GetShaderProgramID() const;

	void SetBool(const std::string& name, bool value) const;

	void SetInt(const std::string& name, int value) const;

	void SetFloat(const std::string& name, float value) const;

	void SetFloat(const std::string& name, float v1, float v2) const;

	void SetFloat(const std::string& name, float v1, float v2, float v3) const;

	void SetFloat(const std::string& name, float v1, float v2, float v3, float v4) const;

	void SetVector2f(const GLchar* name, GLfloat x, GLfloat y) const;

	void SetVector2f(const GLchar* name, const glm::vec2& value) const;

	void SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z) const;

	void SetVector3f(const GLchar* name, const glm::vec3& value) const;

	void SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const;

	void SetVector4f(const GLchar* name, const glm::vec4& value) const;

	void SetMatrix4(const GLchar* name, const glm::mat4& matrix) const;

	static bool _CompileShader(UINT nShaderID, const std::string& szCode);

	static bool _LinkShader(UINT nProgramID, UINT nVertexID, UINT nFragmentID);

	static std::string _GetShaderError(UINT nShaderID);

	static std::string _LoadFile(const std::string& szPath);

private:

	UINT m_ShaderProgramID{};	//shader³ÌÐòid
};

#endif