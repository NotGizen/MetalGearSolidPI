#pragma once
#include "Entity.h"

class Model;
//class Transform;
class Bullet;
class Gun : public Entity
{
public:
	Gun(const std::string& path); // creates bullets
	~Gun();
	void Init();
	void Shoot();
	void Reload(float dt);
	void BulletOut();
	void SetCanShoot(bool value);
	bool GetCanShoot() const;
	unsigned int GetTotalBullets() const;
	unsigned int GetRemainingBullets() const;
	bool GetIsMagazineEmpty() const;
	//Get
	//Model& GetGunModel();

private:
	unsigned int totalBullets;
	unsigned int bulletsRemaining;
	bool canShoot;
	float reloadTime;
	bool magazineEmpty;
	

	//Model* gunModel;
	//Bullet* bullet;
};

