#pragma once
#include "Actor.h"
#include "Entity.h"
class Camera;
class Gun;
class Entity;
class Scene;
class Player : public Actor
{
public:
	enum Action
	{
		Walk,
		Attack,
		Idle
	}action;

	Player(const std::string& path);
	~Player();
	void Init();
	void Update(Camera& camera, Scene& scene) ;
	void MoveInput( Camera& cam, Scene& scene);
	void IncreaseHealth();

	//Get
	const int& GetBulletsMag();
	bool GetIsShooting() const;
	//Model& GetPistol() const;
		
private:
	int bulletsMagazine;
	bool isShooting;
	//Gun* pistol = nullptr;
	
};

