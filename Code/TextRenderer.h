#ifndef __TextRenderer_H__
#define __TextRenderer_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <map>
#include <iostream>


#include "Shader.h"
#include "SpriteRenderer.h"
#include "ResourceMgr.h"

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
	GLuint Advance;     // Horizontal offset to advance to next glyph
};


class CTextRenderer
{
public:
	std::map<GLchar, Character> Characters;
	Shader TextShader;

	CTextRenderer(GLuint width, GLuint height);

	void Load(std::string font, GLuint fontSize);

	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));

private:
	GLuint VAO, VBO;
};


#endif
