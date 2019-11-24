#include "Game.h"

Game::Game(GLuint width, GLuint height)
	: Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
	delete render;
}

void Game::Init()
{
	Shader oShader = ResourceMgr::LoadShader("Assert/Shader/2DSprite.vs", "Assert/Shader/2DSprite.fs", nullptr, "Sprite");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);

	// ÅäÖÃ×ÅÉ«Æ÷
	oShader.UseShader();
	oShader.SetInt("image", 0);
	oShader.SetMatrix4("projection", projection);

	render = new SpriteRenderer(oShader);

	ResourceMgr::LoadTexture("Assert/Res/textures/awesomeface.png", GL_TRUE, "face");

}

void Game::ProcessInput(GLfloat dt)
{
}

void Game::Update(GLfloat dt)
{
}

void Game::Render()
{
	render->DrawSprite(
		ResourceMgr::GetTexture("face"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
