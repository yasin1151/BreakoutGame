#include "Game.h"

const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);
SpriteRenderer* render;
GameObject* Player;
MyParticleMgr* ParticleMgr;


// 球
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
}

void Game::Init()
{

	// 加载纹理
	ResourceMgr::LoadTexture("textures/background.jpg", GL_FALSE, "background");
	ResourceMgr::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
	ResourceMgr::LoadTexture("textures/block.png", GL_FALSE, "block");
	ResourceMgr::LoadTexture("textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceMgr::LoadTexture("textures/paddle.png", GL_TRUE, "paddle");
	ResourceMgr::LoadTexture("textures/particle.png", GL_TRUE, "particle");

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


	// 初始化球
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceMgr::GetTexture("face"));
	
	Shader oShader = ResourceMgr::LoadShader("Assert/Shader/2DSprite.vs", "Assert/Shader/2DSprite.fs", nullptr, "Sprite");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);

	// 配置着色器
	oShader.UseShader();
	oShader.SetInt("image", 0);
	oShader.SetMatrix4("projection", projection);
	render = new SpriteRenderer(oShader);

	oShader = ResourceMgr::LoadShader("Assert/Shader/Particle.vs", "Assert/Shader/Particle.fs", nullptr, "particle");
	oShader.UseShader();
	oShader.SetInt("image", 0);
	oShader.SetMatrix4("projection", projection);
	ParticleMgr = new MyParticleMgr(ResourceMgr::GetShader("particle"), ResourceMgr::GetTexture("particle"), 500);
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
		this->ResetLevel();
		this->ResetPlayer();
	}

	ParticleMgr->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));
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
		if (std::get<0>(collision)) // 如果发生了碰撞
		{
			if (!box.IsSoild)
			{
				box.Destroyed = GL_TRUE;
			}

			// 碰撞后的反弹处理
			Direction dir = std::get<1>(collision);
			glm::vec2 diff_vector = std::get<2>(collision);

			// 水平方向碰撞
			if (dir == LEFT || dir == RIGHT)
			{
				Ball->Velocity.x = -Ball->Velocity.x;	// 反转水平速度
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
			// 垂直方向碰撞
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
		// 发生碰撞
		// 检查碰到了挡板的哪个位置，并根据碰到哪个位置来改变速度
		GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
		GLfloat dis = (Ball->Position.x + Ball->Radius) - centerBoard;
		GLfloat percentage = dis / (Player->Size.x / 2);
		// 依据结果移动
		GLfloat strength = 2.0f;
		glm::vec2 oldVelocity = Ball->Velocity;
		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		Ball->Velocity.y = -Ball->Velocity.y;
		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);

	}

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

		ParticleMgr->Draw();

		Ball->Draw(*render);
	}

}

void Game::ResetLevel()
{
	this->level = 1;
}

void Game::ResetPlayer()
{
	// 初始化玩家属性
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
		glm::vec2(0.0f, 1.0f),  // 上
		glm::vec2(1.0f, 0.0f),  // 右
		glm::vec2(0.0f, -1.0f), // 下
		glm::vec2(-1.0f, 0.0f)  // 左
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
	// 获取圆的中心 
	glm::vec2 center(one.Position + one.Radius);
	// 计算AABB的信息（中心、半边长）
	glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
	glm::vec2 aabb_center(
		two.Position.x + aabb_half_extents.x,
		two.Position.y + aabb_half_extents.y
	);
	// 获取两个中心的差矢量
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// AABB_center加上clamped这样就得到了碰撞箱上距离圆最近的点closest
	glm::vec2 closest = aabb_center + clamped;
	// 获得圆心center和最近点closest的矢量并判断是否 length <= radius
	difference = closest - center;
	if (glm::length(difference) <= one.Radius)
		return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
	return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}
