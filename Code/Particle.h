#ifndef __Particle_H__
#define __Particle_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include "Shader.h"
#include "SpriteRenderer.h"
#include "texture.h"
#include "GameObject.h"

class MyParticleMgr
{
public:

	class ParticleData
	{
	public:
		glm::vec2 Position, Velocity;
		glm::vec4 Color;

		GLfloat Life;

		ParticleData()
			:Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f)
		{}
	};


	MyParticleMgr(Shader shader, Texture2D texture, GLuint amount);

	void Update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));

	void Draw();

private:

	std::vector<ParticleData> Particles;
	GLuint amount;

	Shader shader;
	Texture2D _texture;

	GLuint VAO;
	void init();

	GLuint firstUnusedParticle();

	void respawnParticle(ParticleData& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};


#endif