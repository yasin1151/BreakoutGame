#ifndef __Ball_Object_H__
#define __Ball_Object_H__

#include "GameObject.h"
#include "glm/glm.hpp"

class BallObject : public GameObject
{
public:
	GLfloat Radius;
	GLboolean Stuck;

	BallObject();
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

	glm::vec2 Move(GLfloat dt, GLuint window_width);
	void Reset(glm::vec2 position, glm::vec2 velocity);
};


#endif
