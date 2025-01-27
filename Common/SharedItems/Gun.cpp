#include "Gun.h"
#include "Model.h"
#include "Bullet.h"

Gun::Gun(const std::string &path) : Entity(path), canShoot(false), totalBullets(8), bulletsRemaining(8), reloadTime(2.f), magazineEmpty(false)
{
	
}

Gun::~Gun()
{
	
}

void Gun::Init()
{
	//Entity::transform->position = glm::vec3(0.f, 10.f, 10.f);
	Entity::transform->scale = glm::vec3(1.f, 1.f, 1.f);
	
}

void Gun::Reload(float dt)
{
	if (bulletsRemaining <= 0)
	{
		if (reloadTime <= 0.f)
		{
			bulletsRemaining = 8;
			reloadTime = 2.f;
			magazineEmpty = false;
		}
		else
		{
			magazineEmpty = true;
			reloadTime -= dt;
		}
		
	}
}

void Gun::BulletOut()
{
	bulletsRemaining -= 1;
}

void Gun::SetCanShoot(bool value)
{
	canShoot = value;
}

bool Gun::GetCanShoot() const
{
	return canShoot;
}

unsigned int Gun::GetTotalBullets() const
{
	return totalBullets;
}

unsigned int Gun::GetRemainingBullets() const
{
	return bulletsRemaining;
}

bool Gun::GetIsMagazineEmpty() const
{
	return magazineEmpty;
}

//Model& Gun::GetGunModel()
//{
//	return *gunModel;
//}
