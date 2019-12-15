#include "Game.h"

const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);
GLfloat ShakeTime = 0.0f;
SpriteRenderer* render;
GameObject* Player;
MyParticleMgr* ParticleMgr;
PostProcessor* Effects;
CTextRenderer* TextRenderer;

irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();

// ��
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const GLfloat BALL_RADIUS = 12.5f;
BallObject* Ball;


Game::Game(GLuint width, GLuint height)
	: Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
	SafeDelete(render);
	SafeDelete(Player);
	SafeDelete(Ball);
	SafeDelete(ParticleMgr);
	SafeDelete(Effects);
	SafeDelete(TextRenderer);
	SoundEngine->drop();
}

void Game::Init()
{

	// ��������
	ResourceMgr::LoadTexture("textures/background.jpg", GL_FALSE, "background");
	ResourceMgr::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
	ResourceMgr::LoadTexture("textures/block.png", GL_FALSE, "block");
	ResourceMgr::LoadTexture("textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceMgr::LoadTexture("textures/paddle.png", GL_TRUE, "paddle");
	ResourceMgr::LoadTexture("textures/particle.png", GL_TRUE, "particle");
	ResourceMgr::LoadTexture("textures/powerup_speed.png", GL_TRUE, "powerup_speed");
	ResourceMgr::LoadTexture("textures/powerup_sticky.png", GL_TRUE, "powerup_sticky");
	ResourceMgr::LoadTexture("textures/powerup_increase.png", GL_TRUE, "powerup_increase");
	ResourceMgr::LoadTexture("textures/powerup_confuse.png", GL_TRUE, "powerup_confuse");
	ResourceMgr::LoadTexture("textures/powerup_chaos.png", GL_TRUE, "powerup_chaos");
	ResourceMgr::LoadTexture("textures/powerup_passthrough.png", GL_TRUE, "powerup_passthrough");

	char szPath[1024];
	// ���عؿ�
	for (int i = 1; i <= 4; ++i)
	{
		GameLevel gameLevel;
		sprintf_s(szPath, "Assert/Res/maps/%d.lvl", i);
		gameLevel.Load(szPath, this->Width, this->Height * 0.5);
		Levels.push_back(gameLevel);
	}
	this->level = 1;

	// ��ʼ���������
	glm::vec2 playerPos = glm::vec2(
		this->Width / 2 - PLAYER_SIZE.x / 2,
		this->Height - PLAYER_SIZE.y
	);

	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceMgr::GetTexture("paddle"));


	// ��ʼ����
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceMgr::GetTexture("face"));
	
	Shader oShader = ResourceMgr::LoadShader("Assert/Shader/2DSprite.vs", "Assert/Shader/2DSprite.fs", nullptr, "Sprite");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);

	// ������ɫ��
	oShader.UseShader();
	oShader.SetInt("image", 0);
	oShader.SetMatrix4("projection", projection);
	render = new SpriteRenderer(oShader);

	oShader = ResourceMgr::LoadShader("Assert/Shader/Particle.vs", "Assert/Shader/Particle.fs", nullptr, "particle");
	oShader.UseShader();
	oShader.SetInt("image", 0);
	oShader.SetMatrix4("projection", projection);
	ParticleMgr = new MyParticleMgr(ResourceMgr::GetShader("particle"), ResourceMgr::GetTexture("particle"), 500);


	oShader = ResourceMgr::LoadShader("Assert/Shader/post_processing.vs", "Assert/Shader/post_processing.fs", nullptr, "effects");
	Effects = new PostProcessor(oShader, this->Width, this->Height);


	TextRenderer = new CTextRenderer(this->Width, this->Height);
	TextRenderer->Load("Assert/Font/arial.ttf", 20);

	SoundEngine->play2D("Assert/audio/breakout.mp3", GL_TRUE);
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
		offset += glm::vec2(-1.0f, 0.0f);
	}
	if (KeyIsDown(GLFW_KEY_RIGHT))
	{
		offset += glm::vec2(1.0f, 0.0f);
	}

	Player->SetPos(pos.x + offset.x * velocity, pos.y + offset.y * velocity);

	if (Ball->Stuck)
	{
		glm::vec2 BallPos = Ball->Position;
		Ball->SetPos(BallPos.x + offset.x * velocity, BallPos.y + offset.y * velocity);
	}

	if (KeyIsDown(GLFW_KEY_SPACE))
	{
		Ball->Stuck = false;
	}
}

void Game::Update(GLfloat dt)
{
	Ball->Move(dt, this->Width);

	this->DoCollisions();

	if (Ball->Position.y >= this->Height)
	{
		std::cout << "Game Reset" << std::endl;
		this->ResetLevel();
		this->ResetPlayer();
	}

	ParticleMgr->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));

	if (ShakeTime > 0.0f)
	{
		ShakeTime -= dt;
		if (ShakeTime <= 0.0f)
		{
			Effects->Shake = false;
		}
	}
}

void Game::DoCollisions()
{
	for (GameObject& box : this->Levels[this->level - 1].Bricks)
	{
		if (box.Destroyed)
		{
			continue;
		}

		Collision collision = CheckCollision(*Ball, box);
		if (std::get<0>(collision)) // �����������ײ
		{
			if (!box.IsSoild)
			{
				box.Destroyed = GL_TRUE;
				SoundEngine->play2D("Assert/audio/bleep.mp3", GL_FALSE);

			}
			else
			{
				SoundEngine->play2D("Assert/audio/solid.wav", GL_FALSE);

			}
			ShakeTime = 0.05f;
			Effects->Shake = true;

			// ��ײ��ķ�������
			Direction dir = std::get<1>(collision);
			glm::vec2 diff_vector = std::get<2>(collision);

			// ˮƽ������ײ
			if (dir == LEFT || dir == RIGHT)
			{
				Ball->Velocity.x = -Ball->Velocity.x;	// ��תˮƽ�ٶ�
				GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
				if (dir == LEFT)
				{
					Ball->Position.x += penetration;
				}
				else
				{
					Ball->Position.x -= penetration;
				}
			}
			// ��ֱ������ײ
			else
			{
				Ball->Velocity.y = -Ball->Velocity.y;
				GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
				if (dir == UP)
				{
					Ball->Position.y -= penetration;
				}
				else
				{
					Ball->Position.y += penetration;
				}
			}
		}

	}

	Collision result = CheckCollision(*Ball, *Player);
	if (!Ball->Stuck && std::get<0>(result))
	{
		// ������ײ
		// ��������˵�����ĸ�λ�ã������������ĸ�λ�����ı��ٶ�
		GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
		GLfloat dis = (Ball->Position.x + Ball->Radius) - centerBoard;
		GLfloat percentage = dis / (Player->Size.x / 2);
		// ���ݽ���ƶ�
		GLfloat strength = 2.0f;
		glm::vec2 oldVelocity = Ball->Velocity;
		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		Ball->Velocity.y = -Ball->Velocity.y;
		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
		SoundEngine->play2D("Assert/audio/bleep.wav", GL_FALSE);

	}

}

void Game::Render()
{

	if (this->State == GAME_ACTIVE)
	{
		// ���Ʊ���
		Effects->BeginRender();

		render->DrawSprite(ResourceMgr::GetTexture("background"),
			glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f
		);
		this->Levels[this->level - 1].Draw(*render);

		Player->Draw(*render);

		ParticleMgr->Draw();

		Ball->Draw(*render);

		Effects->EndRender();
		Effects->Render(glfwGetTime());

		TextRenderer->RenderText("Hello Game!", 0, 0, 1.0f);
	}

}

void Game::ResetLevel()
{
	this->level = 1;
}

void Game::ResetPlayer()
{
	// ��ʼ���������
	glm::vec2 playerPos = glm::vec2(
		this->Width / 2 - PLAYER_SIZE.x / 2,
		this->Height - PLAYER_SIZE.y
	);

	Player->Position = playerPos;

	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);

	Ball->Position = ballPos;
	Ball->Velocity = INITIAL_BALL_VELOCITY;
	Ball->Stuck = true;

}

Direction Game::VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),  // ��
		glm::vec2(1.0f, 0.0f),  // ��
		glm::vec2(0.0f, -1.0f), // ��
		glm::vec2(-1.0f, 0.0f)  // ��
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}


bool Game::KeyIsDown(GLuint nKey)
{
	return this->Keys[nKey] && !this->KeysProcessed[nKey];
}

Game::Collision Game::CheckCollision(BallObject& one, GameObject& two)
{
	// ��ȡԲ������ 
	glm::vec2 center(one.Position + one.Radius);
	// ����AABB����Ϣ�����ġ���߳���
	glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
	glm::vec2 aabb_center(
		two.Position.x + aabb_half_extents.x,
		two.Position.y + aabb_half_extents.y
	);
	// ��ȡ�������ĵĲ�ʸ��
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// AABB_center����clamped�����͵õ�����ײ���Ͼ���Բ����ĵ�closest
	glm::vec2 closest = aabb_center + clamped;
	// ���Բ��center�������closest��ʸ�����ж��Ƿ� length <= radius
	difference = closest - center;
	if (glm::length(difference) <= one.Radius)
		return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
	return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}
