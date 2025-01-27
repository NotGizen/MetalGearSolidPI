#include "Player.h"
#include "Camera.h"
#include "Animation.h"
#include "Model.h"
#include "AnimationManager.h"
#include "Camera.h"
#include "SceneRenderer.h"
#include "Gun.h"

Player::Player(const std::string& path) : Actor(path), bulletsMagazine(64), isShooting(false)
{
	health = 7;
	isImmortal = false;
	spawnPosition = glm::vec3(0.f, 0.f, -45.f);	
	//SetPosition(spawnPosition);
	//SetScale(glm::vec3(1.f, 1.f, 1.f));
}

Player::~Player()
{
	
}

void Player::Init()
{
	
	
}

void Player::Update(Camera& camera, Scene& scene)
{
	//Actor::Update();
	MoveInput( camera, scene);
	
	
}

void Player::MoveInput( Camera& cam, Scene& scene)
{
	
		
		
		glm::vec3 movement = glm::vec3(0.f, 0.f, 0.f);
		
	if (cam.type == Camera::FirstPerson)
	{
		
			if (posDir.left)
			{
				movement.x -= cam.Right.x;
				movement.z -= cam.Right.z;
			}
			if (posDir.right)
			{
				movement.x += cam.Right.x;
				movement.z += cam.Right.z;
			}
			if (posDir.front)
			{
				movement.x += cam.Front.x;
				movement.z += cam.Front.z;
			}
			if (posDir.back)
			{
				movement.x -= cam.Front.x;
				movement.z -= cam.Front.z;
			}		
		
	}
	else
	{
		
		if (scene.GetLevel() == Scene::LEVEL2)
		{
			if (posDir.front)
			{
				movement.z -= 1.f;
				rotationDir.z = -1.f;
				rotationDir.x = 0.f;
			}
			if (posDir.back)
			{
				movement.z += 1.f;
				rotationDir.z = 1.f;
				rotationDir.x = 0.f;
			}
			if (posDir.left)
			{
				movement.x -= 1.f;
				rotationDir.x = -1.f;
				rotationDir.z = 0.f;
			}
			if (posDir.right)
			{
				movement.x += 1.f;
				rotationDir.x = 1.f;
				rotationDir.z = 0.f;
			}
		}
		else
		{
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
		}
		
		//SetPosition(position + movement * velocity);

	}
		if (movement != glm::vec3(0.f) )
		{
			
			movement = glm::normalize(movement);
			//rotationDir = glm::normalize(rotationDir);
			
		}
		direction = movement;
		
}

void Player::IncreaseHealth()
{
	health += 1;
}

const int& Player::GetBulletsMag()
{
	return bulletsMagazine;
}

bool Player::GetIsShooting() const
{
	return isShooting;
}

//Model& Player::GetPistol() const
//{
//	return pistol->GetGunModel();
//}







