#ifndef __GameObject_H__
#define __GameObject_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
	// ¶ÔÏó×´Ì¬
	glm::vec2	Position, Size, Velocity;
	glm::vec3	Color;
	GLfloat		Rotation;
	GLboolean	IsSoild;
	GLboolean	Destroyed;

	// äÖÈ¾×´Ì¬
	Texture2D	Sprite;
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

	virtual void Draw(SpriteRenderer& renderer);

	void SetPos(const glm::vec2& vPos);
	void SetPos(float x, float y);

	glm::vec2 GetPos() const;

};


#endif
