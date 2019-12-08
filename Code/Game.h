#ifndef __GAME_H__
#define __GAME_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <tuple>

#include "ResourceMgr.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "GameLevel.h"
#include "BallObject.h"
#include "Particle.h"

enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
};

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

const unsigned int KEY_LEN = 1024;

class Game
{
public:
	typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

	GameState State = GAME_ACTIVE;
	GLboolean Keys[KEY_LEN];
	GLboolean KeysProcessed[KEY_LEN];
	GLuint Width, Height;


	std::vector<GameLevel> Levels;
	GLuint	level;

	Game(GLuint width, GLuint height);
	~Game();

	void Init();

	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void DoCollisions();
	void Render();

	void ResetLevel();
	void ResetPlayer();


	bool KeyIsDown(GLuint nKey);
	Direction VectorDirection(glm::vec2 target);

	Collision CheckCollision(BallObject& one, GameObject& two);

};

#endif
