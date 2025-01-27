#include "Enemy.h"

Enemy::Enemy(const std::string& path) : Actor(path), alarmState(PATROLLING) , canSaveDir(false), isShooting(false)
{
	health = 1;
	if(alarmState == PATROLLING)	speed = 20.f;
	else speed = 40.f;
}

void Enemy::Init()
{
	
}

void Enemy::StopMoving()
{
	if (this != nullptr)
	{

		if (!canSaveDir)
		{
			//save last positions
			savedDir.front = posDir.front;
			savedDir.back = posDir.back;
			savedDir.left = posDir.left;
			savedDir.right = posDir.right;

			posDir.front = false;
			posDir.back = false;
			posDir.left = false;
			posDir.right = false;

			canSaveDir = true;
		}
	}
}

void Enemy::ContinueMoving()
{
	if (this != nullptr)
	{
		if (canSaveDir)
		{
			posDir.front = savedDir.front;
			posDir.back = savedDir.back;
			posDir.left = savedDir.left;
			posDir.right = savedDir.right;
			canSaveDir = false;
		}

	}
}

void Enemy::SetupPatrolling()
{
}

void Enemy::Update()
{
	if (this != nullptr)
	{

		MoveInput();
		switch (alarmState)
		{
		case PATROLLING:
			break;
		case ALLERT:
			break;
		default:
			break;
		}
	}
}

Enemy::AlarmState Enemy::GetState() const
{
	
		return alarmState;

	
}

void Enemy::SetState(int value)
{
	if (value < 0 || value > 1) std::cout << "FAil setting EnemyState" << std::endl;
	else
	{
		alarmState = static_cast<AlarmState>(value);

	}
}

void Enemy::MoveInput()
{
	
	glm::vec3 movement = glm::vec3(0.f, 0.f, 0.f);
	if (posDir.front)
	{
		movement.z += 1.f;
		rotationDir.z = 1.f;
		rotationDir.x = 0.f;
	}
	if (posDir.back)
	{
		movement.z -= 1.f;
		rotationDir.z = -1.f;
		rotationDir.x = 0.f;
	}
	if (posDir.left)
	{
		movement.x += 1.f;
		rotationDir.x = 1.f;
		rotationDir.z = 0.f;
	}
	if (posDir.right)
	{
		movement.x -= 1.f;
		rotationDir.x = -1.f;
		rotationDir.z = 0.f;
	}
	//SetPosition(position + movement * velocity);

	if (movement != glm::vec3(0.f))
	{

		movement = glm::normalize(movement);
		//rotationDir = glm::normalize(rotationDir);

	}
	direction = movement;
}

void Enemy::SetIsShooting(bool value)
{
	isShooting = value;
}

bool Enemy::GetIsShooting() const
{
	return isShooting;
}

