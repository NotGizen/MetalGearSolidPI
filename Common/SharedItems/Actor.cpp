#include "Actor.h"
#include "Model.h"
#include "AnimationManager.h"
#include "Transform.h"
#include "cmath"

Actor::Actor(const std::string& path) : Entity(path), health(3), isAlive(true), changedDirection(false), lastRotationDir(0.f), rotationLerpTimer(0.f), 
	isImmortal(false)

{
	//animator = new AnimationManager();
	
	speed = 30.f;
	//position = model->GetPosition();
	ZAxis = glm::vec3(0.f, 0.f, 1.f);
	XAxis = glm::vec3(1.f, 0.f, 0.f);
	posDir.front = false;
	posDir.back = false;
	posDir.left = false;
	posDir.right = false;
}

Actor::Actor() : health(3), isAlive(true), changedDirection(false), lastRotationDir(0.f), rotationLerpTimer(0.f),
isImmortal(false)
{
	speed = 30.f;
	//position = model->GetPosition();
	ZAxis = glm::vec3(0.f, 0.f, 1.f);
	XAxis = glm::vec3(1.f, 0.f, 0.f);
	posDir.front = false;
	posDir.back = false;
	posDir.left = false;
	posDir.right = false;
}

Actor::~Actor()
{
	//delete model;
	//model = nullptr;
}

void Actor::Update()
{
	//position = model->GetPosition();
	//scale = model->GetScaleRef();
	//angle = model->GetRotationRef();
	//MovingDirection(animator);
}

//void Actor::Draw(Shader& shader)
//{
//	model->Draw(shader);
//}

//void Actor::MovingDirection(AnimationManager& animator)
//{
//	if (!posDir.idle)
//	{
//		if (posDir.left)
//		{
//			SetRotationVector(90.f, animator.GetWalkAnim());
//			lastAngle = 90.f;
//		}
//		if (posDir.right)
//		{
//			SetRotationVector(270.f, animator.GetWalkAnim());
//			lastAngle = 270.f;
//		}
//		if (posDir.front)
//		{
//			SetRotationVector(0.f, animator.GetWalkAnim());
//			lastAngle = 0.f;
//		}
//		if (posDir.front && posDir.left)
//		{
//			SetRotationVector(45.f, animator.GetWalkAnim());
//			lastAngle = 45.f;
//		}
//		if (posDir.front && posDir.right)
//		{
//			SetRotationVector(315.f, animator.GetWalkAnim());
//			lastAngle = 315.f;
//		}
//
//		if (posDir.back)
//		{
//			SetRotationVector(180.f, animator.GetWalkAnim());
//			lastAngle = 180.f;
//		}
//		if (posDir.back && posDir.left)
//		{
//			SetRotationVector(135.f, animator.GetWalkAnim());
//			lastAngle = 135.f;
//		}
//		if (posDir.back && posDir.right)
//		{
//			SetRotationVector(225.f, animator.GetWalkAnim());
//			lastAngle = 225.f;
//		}
//	}
//	else
//	{
//		SetRotationVector(lastAngle, animator.GetWalkAnim());
//	}
	/*switch (dirPos)
	{
	case Actor::UP:
		model->SetRotation(0.f);
		SetRotationVector(0.f, animator.GetWalkAnim());
		break;
	case Actor::DOWN:
		model->SetRotation(180.f);
		SetRotationVector(180.f, animator.GetWalkAnim());
		break;
	case Actor::LEFT:
		model->SetRotation(90.f);
		SetRotationVector(90.f, animator.GetWalkAnim());
		break;
	case Actor::RIGHT:
		model->SetRotation(270.f);
		SetRotationVector(270.f, animator.GetWalkAnim());
		break;
	default:
		break;
	}*/
//}

//void Actor::SetPosition(glm::vec3 vec)
//{
//	model->SetPosition(vec);
//}
//
//
//void Actor::SetScale(glm::vec3 vec)
//{
//	model->SetScale(vec);
//}
//
//void Actor::SetRotation(float angle)
//{
//	model->SetRotation(angle);
//}
//
//void Actor::SetRotationVector(float angle, std::vector<Model*>& vec)
//{
//	for (Model* vecModel : vec)
//	{
//		vecModel->SetRotation(angle);
//	}
//}

void Actor::SetSpeed(float newspeed)
{
	this->speed = newspeed;
}

void Actor::SetIsAlive(bool boolAlive)
{
	this->isAlive = boolAlive;
}

void Actor::SetLastRotationDir(float value)
{
	lastRotationDir = value;
}

//glm::vec3 Actor::GetPosition() const
//{
//	return model->GetPosition();
//}
//
//glm::vec3& Actor::GetPositionRef()
//{
//	return model->GetPositionRef();
//}
//
//glm::vec3& Actor::GetScaleRef()
//{
//	return model->GetScaleRef();
//}
//
//float& Actor::GetRotationRef()
//{
//	return model->GetRotationRef();
//}

//Model& Actor::GetModel()
//{
//	//return *model;
//}

void Actor::SetLerpTimer(float value) 
{
	rotationLerpTimer = value;
}

float Actor::GetLerpTimer()
{
	return rotationLerpTimer;
}

float Actor::CalculateRotation()
{
	float radians = std::atan2(rotationDir.x, rotationDir.z);
	float degrees = glm::degrees(radians);
	
	return degrees;
}

float Actor::GetLastRotation()
{
	return lastRotationDir;
}

glm::vec3 Actor::GetDirection() const
{
	return direction;
}

glm::vec3 Actor::GetSpawnPosition() const
{
	return spawnPosition;
}


float Actor::GetSpeed() const
{
	if (this != nullptr)
	{
		return speed;

	}
	return 0.f;
}

const int& Actor::GetHealth() 
{
	return health;
}

bool Actor::GetIsImmortal() const
{
	return isImmortal;
}

bool Actor::GetIsAlive() const
{
	if (this != nullptr)
	{
		return isAlive;

	}
	return !isAlive;
}

void Actor::ReceiveDamage()
{
	if (this != nullptr)
	{
		health -= 1;
	
		if (health <= 0 && !isImmortal)
		{
			isAlive = false;
		}

	}
}


