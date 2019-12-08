#include "Particle.h"

MyParticleMgr::MyParticleMgr(Shader shader, Texture2D texture, GLuint amount)
	: shader(shader), _texture(texture), amount(amount)
{
	this->init();
}

void MyParticleMgr::Update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset)
{
	GLuint iNewParticle = 2;
	for (GLuint i = 0; i < iNewParticle; ++i)
	{
		int unusedParticle = firstUnusedParticle();
		if (unusedParticle == -1)
		{
			continue;
		}
		respawnParticle(Particles[unusedParticle], object, offset);
	}

	// »ØÊÕ
	for (GLuint i = 0; i < this->amount; ++i)
	{
		ParticleData& p = this->Particles[i];
		p.Life -= dt;
		if (p.Life > 0.0f)
		{
			p.Position -= p.Velocity * dt;
			p.Color.a -= dt * 2.5;
		}
	}

}

void MyParticleMgr::Draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.UseShader();

	for (auto& p : this->Particles)
	{
		if (p.Life > 0.0f)
		{
			this->shader.SetVector2f("offset", p.Position);
			this->shader.SetVector4f("color", p.Color);
			this->_texture.Bind();

			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void MyParticleMgr::init()
{
	GLuint VBO;
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);

	glBindVertexArray(0);

	for (GLuint i = 0; i < this->amount; ++i)
	{
		Particles.push_back(ParticleData());
	}
}

GLuint lastUsedParticle = 0;
GLuint MyParticleMgr::firstUnusedParticle()
{
	for (GLuint i = lastUsedParticle; i < this->amount; ++i)
	{
		if (this->Particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	for (GLuint i = 0; i < lastUsedParticle; ++i)
	{
		if (this->Particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	lastUsedParticle = 0;
	return 0;
}

void MyParticleMgr::respawnParticle(ParticleData& particle, GameObject& object, glm::vec2 offset)
{
	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
	particle.Position = object.Position + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = object.Velocity * 0.1f;
}

