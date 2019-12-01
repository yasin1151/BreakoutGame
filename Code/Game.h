#ifndef __GAME_H__
#define __GAME_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "ResourceMgr.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "GameLevel.h"

enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
};


const unsigned int KEY_LEN = 1024;

class Game
{
public:
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
	void Render();

	bool KeyIsDown(GLuint nKey);
};

#endif
