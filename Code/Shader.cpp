#include "Shader.h"


Shader::Shader(): m_ShaderProgramID(0)
{
}

Shader::Shader(const std::string& szVertexPath, const std::string& szFragmentPath)
{
	InitWithFile(szVertexPath, szFragmentPath);
}

bool Shader::InitWithFile(const std::string& szVertexPath, const std::string& szFragmentPath)
{
	if (szVertexPath.empty() || szFragmentPath.empty())
	{
		return false;
	}

	std::string szVertexCode = _LoadFile(szVertexPath);
	std::string szFragmentCode = _LoadFile(szFragmentPath);
	return InitWithCode(szVertexCode, szFragmentCode);
}

bool Shader::InitWithCode(const std::string& szVertexCode, const std::string& szFragmentCode)
{
	if (szVertexCode.empty() || szFragmentCode.empty())
	{
		return false;
	}

	UINT nVertexID = glCreateShader(GL_VERTEX_SHADER);
	if (!_CompileShader(nVertexID, szVertexCode))
	{
		std::cout << _GetShaderError(nVertexID) << std::endl;
		return false;
	}

	UINT nFragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	if (!_CompileShader(nFragmentID, szFragmentCode))
	{
		std::cout << _GetShaderError(nFragmentID) << std::endl;
		return false;
	}

	m_ShaderProgramID = glCreateProgram();
	if (!_LinkShader(m_ShaderProgramID, nVertexID, nFragmentID))
	{
		std::cout << _GetShaderError(m_ShaderProgramID) << std::endl;
		return false;
	}

	glDeleteShader(nVertexID);
	glDeleteShader(nFragmentID);

	return true;
}

void Shader::UseShader() const
{
	glUseProgram(m_ShaderProgramID);
}

Shader::UINT Shader::GetShaderProgramID() const
{
	return m_ShaderProgramID;
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_ShaderProgramID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_ShaderProgramID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_ShaderProgramID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float v1, float v2) const
{
	glUniform2f(glGetUniformLocation(m_ShaderProgramID, name.c_str()), v1, v2);
}

void Shader::SetFloat(const std::string& name, float v1, float v2, float v3) const
{
	glUniform3f(glGetUniformLocation(m_ShaderProgramID, name.c_str()), v1, v2, v3);
}

void Shader::SetFloat(const std::string& name, float v1, float v2, float v3, float v4) const
{
	glUniform4f(glGetUniformLocation(m_ShaderProgramID, name.c_str()), v1, v2, v3, v4);
}

void Shader::SetVector2f(const GLchar* name, GLfloat x, GLfloat y) const
{
	glUniform2f(glGetUniformLocation(m_ShaderProgramID, name), x, y);
}

void Shader::SetVector2f(const GLchar* name, const glm::vec2& value) const
{
	glUniform2f(glGetUniformLocation(m_ShaderProgramID, name), value.x, value.y);
}

void Shader::SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z) const
{
	glUniform3f(glGetUniformLocation(m_ShaderProgramID, name), x, y, z);
}

void Shader::SetVector3f(const GLchar* name, const glm::vec3& value) const
{
	glUniform3f(glGetUniformLocation(m_ShaderProgramID, name), value.x, value.y, value.z);
}

void Shader::SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
{
	glUniform4f(glGetUniformLocation(m_ShaderProgramID, name), x, y, z, w);
}

void Shader::SetVector4f(const GLchar* name, const glm::vec4& value) const
{
	glUniform4f(glGetUniformLocation(m_ShaderProgramID, name), value.x, value.y, value.z, value.z);
}

void Shader::SetMatrix4(const GLchar* name, const glm::mat4& matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

bool Shader::_CompileShader(UINT nShaderID, const std::string& szCode)
{
	if (szCode.empty() || !nShaderID)
	{
		return false;
	}

	const char* szCStr = szCode.c_str();
	int iSucc = 0;
	glShaderSource(nShaderID, 1, &szCStr, nullptr);
	glCompileShader(nShaderID);
	glGetShaderiv(nShaderID, GL_COMPILE_STATUS, &iSucc);
	return bool(iSucc);
}

bool Shader::_LinkShader(UINT nProgramID, UINT nVertexID, UINT nFragmentID)
{
	if (!nProgramID || !nVertexID || !nFragmentID)
	{
		return false;
	}

	glAttachShader(nProgramID, nVertexID);
	glAttachShader(nProgramID, nFragmentID);
	glLinkProgram(nProgramID);
	int iSucc = 0;
	glGetProgramiv(nProgramID, GL_LINK_STATUS, &iSucc);
	return bool(iSucc);
}

std::string Shader::_GetShaderError(UINT nShaderID)
{
	char szInfoLog[SHADER_LOG_BUF_SIZE];
	glGetShaderInfoLog(nShaderID, SHADER_LOG_BUF_SIZE, nullptr, szInfoLog);
	return std::string(szInfoLog);
}

std::string Shader::_LoadFile(const std::string& szPath)
{
	std::string szRet;
	if (szPath.empty())
	{
		return szRet;
	}

	std::ifstream oFileLoader;
	oFileLoader.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		oFileLoader.open(szPath);

		std::stringstream oStringSteam;
		oStringSteam << oFileLoader.rdbuf();

		oFileLoader.close();

		szRet = oStringSteam.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "_LoadFile Error :" << e.what() << std::endl;
		szRet = "";
	}
	return szRet;
}
