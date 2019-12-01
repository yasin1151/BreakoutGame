#include "Game.h"

const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);
SpriteRenderer* render;
GameObject* Player;


Game::Game(GLuint width, GLuint height)
	: Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
	if (render)
	{
		delete render;
	}
	if (Player)
	{
		delete Player;
	}
}

void Game::Init()
{

	// 加载纹理
	ResourceMgr::LoadTexture("textures/background.jpg", GL_FALSE, "background");
	ResourceMgr::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
	ResourceMgr::LoadTexture("textures/block.png", GL_FALSE, "block");
	ResourceMgr::LoadTexture("textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceMgr::LoadTexture("textures/paddle.png", GL_TRUE, "paddle");
	char szPath[1024];
	// 加载关卡
	for (int i = 1; i <= 4; ++i)
	{
		GameLevel gameLevel;
		sprintf_s(szPath, "Assert/Res/maps/%d.lvl", i);
		gameLevel.Load(szPath, this->Width, this->Height * 0.5);
		Levels.push_back(gameLevel);
	}
	this->level = 1;

	// 初始化玩家属性
	glm::vec2 playerPos = glm::vec2(
		this->Width / 2 - PLAYER_SIZE.x / 2,
		this->Height - PLAYER_SIZE.y
	);

	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceMgr::GetTexture("paddle"));

	
	Shader oShader = ResourceMgr::LoadShader("Assert/Shader/2DSprite.vs", "Assert/Shader/2DSprite.fs", nullptr, "Sprite");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);

	// 配置着色器
	oShader.UseShader();
	oShader.SetInt("image", 0);
	oShader.SetMatrix4("projection", projection);
	render = new SpriteRenderer(oShader);
}

void Game::ProcessInput(GLfloat dt)
{
	if (this->State != GAME_ACTIVE)
	{
		return;
	}

	glm::vec2 pos = Player->GetPos();
	glm::vec2 offset = glm::vec2(0.0f);
	GLfloat velocity = PLAYER_VELOCITY * dt;

	if (KeyIsDown(GLFW_KEY_LEFT))
	{
		offset = glm::vec2(-1.0f, 0.0f);
	}
	else if (KeyIsDown(GLFW_KEY_RIGHT))
	{
		offset = glm::vec2(1.0f, 0.0f);
	}

	else if (KeyIsDown(GLFW_KEY_UP))
	{
		offset = glm::vec2(0.0f, -1.0f);
	}
	else if (KeyIsDown(GLFW_KEY_DOWN))
	{
		offset = glm::vec2(0.0f, 1.0f);
	}
	Player->SetPos(pos.x + offset.x * velocity, pos.y + offset.y * velocity);
}

void Game::Update(GLfloat dt)
{
}

void Game::Render()
{

	if (this->State == GAME_ACTIVE)
	{
		// 绘制背景
		render->DrawSprite(ResourceMgr::GetTexture("background"),
			glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f
		);
		this->Levels[this->level - 1].Draw(*render);

		Player->Draw(*render);
	}

}

bool Game::KeyIsDown(GLuint nKey)
{
	if (this->Keys[nKey] && !this->KeysProcessed[nKey])
	{
		return true;
	}
	return false;
}
