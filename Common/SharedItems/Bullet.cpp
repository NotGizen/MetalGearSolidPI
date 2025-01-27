#include "Bullet.h"
#include "Model.h"
#include <LinearMath/btVector3.h>

Bullet::Bullet(std::string const& path) : Entity(path), speed(20.f)
{
	//bulletModel = new Model("../Common/Assets/Items/Bullet/9x19mm.obj");
}

Bullet::~Bullet()
{
	delete bulletModel;
	bulletModel = nullptr;
}

void Bullet::Init()
{
	
}


void Bullet::FindTarget(glm::vec3 origin, glm::vec3 target)
{
	
	glm::vec3 direction = target - origin;
	directionNormalized = glm::normalize(direction);
}



Model& Bullet::GetBulletModel()
{
	return *bulletModel;
}

float Bullet::GetSpeed() const
{
	return speed;
}
