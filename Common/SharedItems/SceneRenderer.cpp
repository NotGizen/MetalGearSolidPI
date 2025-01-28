#include "SceneRenderer.h"
#include "Common.h"
#include "Player.h"
#include "Enemy.h"
#include "AnimationManager.h"
#include "Light.h"
#include "Gun.h"
#include "Physics.h"
#include "Bullet.h"
#include "Render.h"

Scene::Scene() : nrLights(2), isCameraLerping(false), lerpTimer(0.f), rotationLerpTimer(0.f), nrPathPoints(4), canShoot(true), isChangingRoom(false), level(Level::LEVEL1), isLevel2Setup(false)
,isExitDoorLoaded(false), didUserWin(false), changingRoomCounter(0), raycastTimer(0.f), playerGunTimer(0.f), isFire(false), playerAnimFrame(10)
{
	
	//ENTITIES
	//player = new Player("../Common/Assets/Naked_Snake/Naked_Snake.obj");
	player = new Player();
	camera = new Camera(glm::vec3(0.f, 10.f, 5.f));
	sceneRoot = new Entity();
	playerCollider = new Entity();
	room = new Entity("../Common/Assets/Level1/level1R.obj");
	gun = new Gun("../Common/Assets/Items/Pistol/Pistol_02.obj");
	playerAnimator = new Animation();
	
	
	
	//physics
	physics = new Physics(room->GetModel());
	
	for (int i = 0; i < nrLights; i++)
	{
		Light* light = new Light();
		lights.push_back(light);
	}

	
	
}

Scene::~Scene()
{
	//delete ScenePhysics
	for (BulletStruct bullet : bullets)
	{
		if (bullet.body != nullptr)
		{
			physics->GetWorld().removeRigidBody(bullet.body);
			btDefaultMotionState* motionState = dynamic_cast<btDefaultMotionState*>(bullet.body->getMotionState());
			delete motionState;

			delete bullet.body;
			delete bullet.entity;
		}
	}
	
	//-------------------//
	delete physics;
	delete sceneRoot;
	delete room;
	delete player;
	for (EnemyStruct& enemy : enemies)
	{
		delete enemy.entity;
		
		delete enemy.animator;
	}
	enemies.clear();
	delete gun;
	delete playerCollider;
	delete camera;

	//delete camera;
	camera = nullptr;
	//delete player;
	player = nullptr;
	
	

	for (Light* i : lights)
	{
		delete i;
		i = nullptr;
	}
	
	delete playerAnimator;
	

}

void Scene::Init(Render& render)
{
	std::cout << "Scene Init" << std::endl;
	//Physics---------
	physics->Init(*player);
	SetUpEntities();
	CreateEnemy(btVector3(-3.f, 8.f, 90.f));
	CreateEnemy(btVector3(-60.f, 8.f, 20.f));


	//Enemy 1
	CreatePathBox(btVector3(-1.f, 0.f, 20.f));
	CreatePathBox(btVector3(-1.f, 0.f, 90.f));
	CreatePathBox(btVector3(55.f, 0.f, 90.f));
	CreatePathBox(btVector3(55.f, 0.f, 20.f));

	//Enemy 2
	CreatePathBox(btVector3(-60.f, 0.f, 20.f));
	CreatePathBox(btVector3(-7.f, 0.f, 20.f));
	CreatePathBox(btVector3(-7.f, 0.f, 90.f));
	CreatePathBox(btVector3(-60.f, 0.f, 90.f));
	
	//------------------------------//
	camera->Init(*this);
	//Textures
	door = render.LoadTexture("../Common/Assets/Items/Room/door.jpg");
	doorBody = physics->CreateDefaultBox(btVector3(20.f, 10.f, 1.f), btVector3(-17.f, 10.f, 133.f));
	exitDoor = render.LoadTexture("../Common/Assets/Items/Room/ExitDoor.png");
	exitDoorBody = physics->CreateDefaultBox(btVector3(10.f, 10.f, 0.1f), btVector3(-7.f, 10.f, -58.f));
	shotTexture = render.LoadTexture("../Common/Assets/Textures/Shot.jpg");
	lights[0]->SetPosition(glm::vec3(0.f, 30.f, 0.f));
	lights[1]->lightType = LightType::Flickering;
	lights[1]->SetPosition(glm::vec3(0.f, 30.f, 50.f));
	
	
	
}



bool Scene::GetDidUserWin() const
{
	return didUserWin;
}

Scene::Level Scene::GetLevel() const
{
	return level;
}

bool Scene::GetIsChangingRoom() const
{
	return isChangingRoom;
}

Physics& Scene::GetPhysics() const
{
	return *physics;
}

Entity& Scene::GetRoom() const
{
	return *room;
}

Camera& Scene::GetCamera() const
{
	return *camera;
}

Camera* Scene::GetCameraP() 
{
	return camera;
}

Player& Scene::GetPlayer() const
{
	return *player;
}

Entity& Scene::GetPlayerCollider() const
{
	return *playerCollider;
}

Gun& Scene::GetGun() const
{
	return *gun;
}



std::vector<Light*>& Scene::GetLights()
{
	return lights;
}

void Scene::DeleteBullet()
{
	
	
	for (std::vector<BulletStruct>::iterator it = bullets.begin(); it != bullets.end();)
	{
		if (it->body != nullptr)
		{
			if (it->isPlayer == false)
			{
				if (physics->DetectDefaultCollision(physics->GetRoomBody(), it->body)
					|| physics->DetectDefaultCollision(physics->GetPlayerRigidBody(), it->body))
				{
					if (physics->DetectDefaultCollision(physics->GetPlayerRigidBody(), it->body))
					{
						//player->ReceiveDamage();
						
					}

					// Delete and erase the bullet
					btDefaultMotionState* motionState = dynamic_cast<btDefaultMotionState*>(it->body->getMotionState());
					delete motionState;
					physics->GetWorld().removeRigidBody(it->body);
					delete it->body;

					room->RemoveUnusedChildren(it->entity);
					it->entity->SetParentToNull();
					delete it->entity;
					it = bullets.erase(it);
					continue; // Skip incrementing `it`
				}
			}
			else
			{
				bool bulletErased = false;

				for (EnemyStruct& enemy : enemies)
				{
					if (physics->DetectDefaultCollision(physics->GetRoomBody(), it->body)
						|| physics->DetectDefaultCollision(enemy.body, it->body))
					{
						if (physics->DetectDefaultCollision(enemy.body, it->body))
						{
							enemy.entity->ReceiveDamage();
						}

						// Delete and erase the bullet
						btDefaultMotionState* motionState = dynamic_cast<btDefaultMotionState*>(it->body->getMotionState());
						delete motionState;
						physics->GetWorld().removeRigidBody(it->body);
						delete it->body;

						room->RemoveUnusedChildren(it->entity);
						it->entity->SetParentToNull();
						delete it->entity;
						it = bullets.erase(it);

						bulletErased = true;
						break; // Exit the inner loop as the bullet is erased
					}
				}

				if (bulletErased)
				{
					continue; // Skip incrementing `it`
				}
			}
		}

		// Increment the iterator if no erase occurred
		++it;
	}
}


void Scene::UpdateScenePhysics(float dt)
{
	if (physics->DetectDefaultCollision(physics->GetPlayerRigidBody(), doorBody) && changingRoomCounter < 1)
	{
		level = LEVEL2;
		SetupLevel2();
		isChangingRoom = true;
		isExitDoorLoaded = true;
		changingRoomCounter = 1;
	}
	if (isChangingRoom)
	{
		if (changingLevelTimer >= 1.f)
		{
			CreateEnemy(btVector3(-10.f, 8.f, 90.f));
			CreateEnemy(btVector3(-3.f, 8.f, 90.f));
			changingLevelTimer = 0.f;
			isChangingRoom = false;
		}
		changingLevelTimer += dt;
	}
	if (isExitDoorLoaded)
	{
		if(physics->DetectDefaultCollision(physics->GetPlayerRigidBody(), exitDoorBody))
		{
			didUserWin = true;
		}
	}
	
	for (EnemyStruct& enemy : enemies)
	{

		physics->DetectCollision(pathBoxes[0], enemy.body, &Physics::MoveUp, *enemy.entity);
		physics->DetectCollision(pathBoxes[1], enemy.body, &Physics::MoveLeft, *enemy.entity);
		physics->DetectCollision(pathBoxes[2], enemy.body, &Physics::MoveDown, *enemy.entity);
		physics->DetectCollision(pathBoxes[3], enemy.body, &Physics::MoveRight, *enemy.entity);

		physics->DetectCollision(pathBoxes[4], enemy.body, &Physics::MoveLeft, *enemy.entity);
		physics->DetectCollision(pathBoxes[5], enemy.body, &Physics::MoveUp, *enemy.entity);
		physics->DetectCollision(pathBoxes[6], enemy.body, &Physics::MoveRight, *enemy.entity);
		physics->DetectCollision(pathBoxes[7], enemy.body, &Physics::MoveDown, *enemy.entity);
		
		//Alert----------------------//
		switch (enemy.entity->GetState())
		{
			case Enemy::PATROLLING:
			{
				glm::vec3 oldEnemyPos = enemy.entity->transform->position;
				
				btVector3 enemyPos = btVector3(oldEnemyPos.x, 15.f, oldEnemyPos.z);

				if (enemy.entity->posDir.front)
				{
					if (physics->PerformPatrollingRaycast(enemyPos, enemyPos + btVector3(0.f, 0.f, 50.f),
						btVector3(1.f, 1.f, 0.f), CollisionTypes::PLAYER))
					{
						for (EnemyStruct& e : enemies)
						{
							e.entity->SetState(1);
							camera->SetIsTypeChanged(true);
							camera->type = Camera::FirstPerson;
						}

					}
				}
				if (enemy.entity->posDir.back)
				{
					if (physics->PerformPatrollingRaycast(enemyPos, enemyPos + btVector3(0.f, 0.f, -50.f),
						btVector3(1.f, 1.f, 0.f), CollisionTypes::PLAYER))
					{
						for (EnemyStruct& e : enemies)
						{
							e.entity->SetState(1);
							camera->SetIsTypeChanged(true);
							camera->type = Camera::FirstPerson;
						}
					}
				}
				if (enemy.entity->posDir.left)
				{
					if (physics->PerformPatrollingRaycast(enemyPos, enemyPos + btVector3(50.f, 0.f, 0.f),
						btVector3(1.f, 1.f, 0.f), CollisionTypes::PLAYER))
					{
						for (EnemyStruct& e : enemies)
						{
							
							e.entity->SetState(1);
							camera->SetIsTypeChanged(true);
							camera->type = Camera::FirstPerson;
						}
					}
				}
				if (enemy.entity->posDir.right)
				{
					if (physics->PerformPatrollingRaycast(enemyPos, enemyPos + btVector3(-50.f, 0.f, 0.f),
						btVector3(1.f, 1.f, 0.f), CollisionTypes::PLAYER))
					{
						for (EnemyStruct& e : enemies)
						{
							
							e.entity->SetState(1);
							camera->SetIsTypeChanged(true);
							camera->type = Camera::FirstPerson;
						}
					}
				}
				break;
			}
			case Enemy::ALLERT:
			{
				if (enemy.entity != nullptr)
				{
					for (Light* vecLights : lights)
					{
						vecLights->Update(dt);

					}
					btVector3 oldEBody = enemy.body->getWorldTransform().getOrigin();
					btVector3 enemyBody = btVector3(oldEBody.x(), 15.f, oldEBody.z());
					btVector3 oldPBody = physics->GetPlayerRigidBody()->getWorldTransform().getOrigin();
					btVector3 playerBody = btVector3(oldPBody.x(), 15.f, oldPBody.z());
					
					if (physics->PerformRaycast(oldEBody, oldPBody, btVector3(0.f, 1.f, 1.f), CollisionTypes::WALL))
					{
						if (&enemy != nullptr)
						{

						enemy.entity->ContinueMoving();
						physics->MoveContinue(*enemy.body, *enemy.entity);
						enemy.timer = 0.f;
						}

					}
					else 
					{
						/*glm::vec3 direction = glm::normalize(playerPos - enemyPos);
						float yRotation = glm::degrees(atan2(direction.x, direction.z));
						glm::vec3 texturePosition = glm::vec3(direction.x * 5.f, 13.f, direction.z * 5.f);*/
						glm::vec3 origin = physics->GetBodyPositon(*enemy.body);
						glm::vec3 target = physics->GetBodyPositon(*physics->GetPlayerRigidBody());
						glm::vec3 direction = glm::normalize(target - origin);
						float yRotation = glm::degrees(atan2(direction.x, direction.z));
						enemy.entity->transform->rotation.y = yRotation;
						
						enemy.entity->StopMoving();
						physics->MoveNone(*enemy.body);
						if (enemy.timer > 1.f)
						{
							enemy.entity->SetIsShooting(true);
							CreateBullet(true, enemy.entity, enemy.body);
							if (physics->PerformRaycast(enemy.body->getWorldTransform().getOrigin(), physics->GetbtBodyPosition(*physics->GetPlayerRigidBody()),
								btVector3(1.f, 0.5f, 0.f), PLAYER))
							{
								previousPlayerPos = physics->GetbtBodyPosition(*physics->GetPlayerRigidBody());
								enemy.bulletHit = true;
								player->ReceiveDamage();
							}
							enemy.timer = 0.f;
						}
						else
						{
							if (enemy.timer > 0.9f)
							{
								enemy.entity->SetIsShooting(true);
							}
							else
							{
								enemy.entity->SetIsShooting(false);

							}
								enemy.timer += dt;

						}
					}
				}
				if (enemy.bulletHit)
				{
					
					if (raycastTimer > 1.f)
					{
						raycastTimer = 0.f;
						enemy.bulletHit = false;
					}
					else
					{
						physics->PerformNoCollisionRaycast(enemy.body->getWorldTransform().getOrigin(), previousPlayerPos,
							btVector3(1.f, 0.5f, 0.f));
						raycastTimer += dt;
					}
				}
				
				break;

			}
		}
	}
	DeleteBullet();
//----------------------------------//
}


void Scene::SetupLevel2()
{
	if (!isLevel2Setup)
	{
		camera->type = Camera::ThirdPerson;
		camera->SetIsTypeChanged(true);
		for (EnemyStruct& enemy : enemies)
		{
			if (enemy.entity != nullptr)
			{
				enemy.entity->SetIsAlive(false);
			}
		}
		DestroyEnemy();
		enemies.clear();
		
		//CreateEnemy(btVector3(-3.f, 8.f, 90.f));
		
		
		isLevel2Setup = true;
	}
}

void Scene::SetUpEntities()
{
	
	if (camera->type == Camera::FirstPerson)
	{
		//Add Children
		
		sceneRoot->AddChild(room);
		room->AddChild(playerCollider);
		playerCollider->AddChild(camera);
		camera->AddChild(gun);
		
		
		//Setup
		camera->transform->position = camera->GetOffset();
		player->transform->position.y = -20.f;
		player->transform->rotation.y = 90.f;
		gun->transform->position = glm::vec3(3.f, -2.f,-7.f );
		//sceneRoot->UpdateSelfAndChild();

	}
	else if (camera->type == Camera::ThirdPerson)
	{
		//Add Children
		
		sceneRoot->AddChild(room);
		room->AddChild(playerCollider);
		//room->AddChild(enemy);
		playerCollider->AddChild(camera);
		playerCollider->AddChild(player);
		player->AddChild(gun);

		//Load animations in vectors-----------
		playerAnimator->LoadAnimation("../Common/Assets/Standard_Walking/Standard_Walking", 29);
		
		for (Entity* entity : playerAnimator->GetModelFrame())
		{
			//entity->transform->position = glm::vec3(0.f, 10.f, 0.f);
			entity->transform->scale = glm::vec3(100.f, 100.f, 100.f);
		}
		
		player->AddChild(playerAnimator->GetModelFrame()[playerAnimator->GetFrame()]);
		//------------------------------------------
		//Setup
		
		//camera->transform->position = camera->GetOffset();
		camera->transform->position = glm::vec3(0.f, 65.f, -20.f);
		gun->transform->position = glm::vec3(3.f, 10.f, 4.f);
		gun->transform->rotation.y = 180.f;
	}
		sceneRoot->UpdateSelfAndChild();
}

void Scene::CameraChangeSetup()
{
	//It changed to
	if (camera->type == Camera::FirstPerson)
	{
		//camera->transform->position = glm::vec3(0.f, 20.f, 0.f);
		//Remove
		gun->RemoveAllChildren();
		player->RemoveAllChildren();
		playerCollider->RemoveAllChildren();

		//ADD
		playerCollider->AddChild(camera);
		camera->AddChild(gun);
		gun->transform->position = glm::vec3(3.f, -2.f, -7.f);
		gun->transform->rotation.y = 0.f;
	}
	else if (camera->type == Camera::ThirdPerson)
	{
		camera->transform->position =  glm::vec3(0.f, 65.f, 40.f);
		camera->RemoveAllChildren();

		playerCollider->AddChild(player);
		player->AddChild(gun);
	}
}

void Scene::UpdateEntities(float dt, glm::vec3 colliderPosition)
{
	if (camera->GetIsTypeChanged() == true)
	{
		CameraChangeSetup();
		isCameraLerping = true;
		camera->SetIsTypeChanged(false);
	}
	if (isCameraLerping)
	{
		CameraLerping(dt);
	}
	UpdateBullet(dt);
	playerCollider->transform->position = colliderPosition;
	for (EnemyStruct& enemy : enemies)
	{
		if (enemy.entity)
		{
			enemy.entity->transform->position = physics->GetBodyPositon(*enemy.body);
			if (enemy.entity->GetState() == Enemy::PATROLLING)
			{
			ActorDirectionLerping(dt, *enemy.entity);

			}

		}
	}
	//ANIMATION PLAY
	for (EnemyStruct& enemy : enemies)
	{
		enemy.animator->Play(*enemy.entity, 40);
	}
	if (!player->posDir.idle)
	{	
		playerAnimator->Play(*player, 27);
	}
	else
	{
		playerAnimator->Idle(*player);
	}
	ActorDirectionLerping(dt, *player);
	
	sceneRoot->UpdateSelfAndChild();
}

void Scene::Update(float dt)
{
	physics->Update(dt, *player);
	UpdateEntities(dt, physics->GetBodyPositon(*(physics->GetPlayerRigidBody())));
	
	//--------PHYSICS-------//
	UpdateScenePhysics(dt);
	//
	//--------------------//
	ShootPlayerGun();
	player->Update(*camera, *this);
	DestroyEnemy();
	camera->Update( playerCollider->transform->position, *this);
	for (EnemyStruct& enemy : enemies)
	{
		enemy.entity->Update();
	}
	
	/*for (Light* vecLights : lights)
	{
		vecLights->Update(dt);

	}*/
}

void Scene::UpdateBullet(float dt)
{
	gun->Reload(dt);
	for (BulletStruct bullet : bullets)
	{
		if (!bullet.isPlayer)
		{
		glm::vec3 newPosition = physics->GetBodyPositon(*bullet.body);
		newPosition.y = 10.f;
		bullet.entity->transform->position = newPosition;

		}
		else
		{
			btVector3 pos = bullet.body->getWorldTransform().getOrigin();
			glm::vec3 newpos = glm::vec3(pos.x(), pos.y(), pos.z());
			bullet.entity->transform->position = newpos;
		}
	}
	
}

void Scene::Draw(Shader& shader, Render& render, float dt)
{
	//playerAnimator->Play(*player);
	sceneRoot->DrawSelfAndChildren(shader);
	render.RenderTexture(door, *camera, render.GetTextureShader(), glm::vec3(3.f, 10.f, 133.f), 180.f, glm::vec3(0.f, 0.f, 1.f), glm::vec3(20.f, 30.f, 0.f));
	render.RenderTexture(door, *camera, render.GetTextureShader(), glm::vec3(-17.f, 10.f, 133.f), 180.f, glm::vec3(0.f, 0.f, 1.f), glm::vec3(-20.f, 30.f, 0.f));
	render.RenderTexture(exitDoor, *camera, render.GetTextureShader(), glm::vec3(-7.f, 10.f, -60.f), 180.f, glm::vec3(0.f, 0.f, 1.f), glm::vec3(-30.f, 50.f, 0.f));
	if (isFire)
	{
		glm::vec4 worldGunPos = gun->transform->modelMatrix * glm::vec4(gun->transform->position, 1.f);
		glm::vec3 GunPos = glm::vec3(worldGunPos.x, worldGunPos.y, worldGunPos.z);
		//glm::vec3 offset = glm::vec3(0.3f, 0.f, 0.3f);
		render.RenderTexture(shotTexture, *camera, render.GetTextureShader(), GunPos ,
			camera->transform->rotation.y, glm::vec3(0.f, 1.f, 0.f), glm::vec3(3.f, 3.f, 3.f));
		if (playerGunTimer > 0.2f)
		{
			playerGunTimer = 0.f;
			isFire = false;
		}
		else
		{
			playerGunTimer += dt;
		}
	}
	for (EnemyStruct& enemy : enemies)
	{
		if (enemy.entity != nullptr)
		{
			if (enemy.entity->GetIsShooting())
			{
				glm::vec3 playerPos = glm::vec3(physics->GetbtBodyPosition(*physics->GetPlayerRigidBody()).x(), physics->GetbtBodyPosition(*physics->GetPlayerRigidBody()).y(),
					physics->GetbtBodyPosition(*physics->GetPlayerRigidBody()).z());
				glm::vec3 enemyPos = glm::vec3(enemy.body->getWorldTransform().getOrigin().x(), enemy.body->getWorldTransform().getOrigin().y(), enemy.body->getWorldTransform().getOrigin().z());
				glm::vec3 direction = glm::normalize(playerPos - enemyPos);
				float yRotation = glm::degrees(atan2(direction.x, direction.z));
				glm::vec3 texturePosition = glm::vec3(direction.x * 5.f, 13.f, direction.z * 5.f);
				render.RenderTexture(shotTexture, *camera, render.GetTextureShader(), enemy.entity->transform->position + texturePosition,
					yRotation, glm::vec3(0.f, 1.f, 0.f), glm::vec3(3.f, 3.f, 3.f));
			}

		}
	}
	 
	//Animation Draw
	
	//if (playerAnimFrame > 27)
	//{
	//	playerAnimFrame = 1;
	//}
	//
	//playerAnimation[playerAnimFrame]->DrawSelfAndChildren(shader);
	//std::cout << playerAnimation[playerAnimFrame]->transform->scale.x << std::endl;
	////playerAnimFrame++;
}

void Scene::CameraLerping(float dt)
{
	glm::vec3 newCameraPos = glm::vec3(0.f,0.f,0.f);
	if (camera->type == Camera::FirstPerson)
	{
		newCameraPos = camera->LerpPosition(glm::vec3(0.f, 65.f, -40.f), glm::vec3(0.f, 15.f, 0.f), lerpTimer);
	}
	else if (camera->type == Camera::ThirdPerson)
	{
		if (level == LEVEL2)
		{
			newCameraPos = camera->LerpPosition(glm::vec3(0.f, 15.f, 0.f), glm::vec3(0.f, 65.f, 40.f), lerpTimer);
		}
		else
		{
		newCameraPos = camera->LerpPosition(glm::vec3(0.f, 15.f, 0.f), glm::vec3(0.f, 65.f, -40.f), lerpTimer);

		}
	}

	camera->transform->position = newCameraPos;
	lerpTimer += 5 * dt;
	if (lerpTimer >= 1.f)
	{
		lerpTimer = 1.f;
		camera->transform->position = newCameraPos;
		lerpTimer = 0.f;
		isCameraLerping = false;
	}
}

void Scene::ActorDirectionLerping(float dt, Actor& actor)
{
	if (player == &actor && camera->type == Camera::FirstPerson) return;
	else
	{
		float newDir = actor.CalculateRotation();
		float currentDir = actor.GetLastRotation();
		float delta = newDir - currentDir;
		if (delta == 0)
		{
			actor.SetLerpTimer(0.f);
		}
		
		if (delta > 180.0f)
			delta -= 360.0f;
		else if (delta < -180.0f)
			delta += 360.0f;
		//std::cout << newDir << std::endl;

		actor.transform->rotation.y = currentDir + glm::mix(0.f, delta , actor.GetLerpTimer());
		
		float lerpValue = actor.GetLerpTimer();
		actor.SetLerpTimer(lerpValue += 10 * dt);
		if (actor.GetLerpTimer() >= 1.f)
		{
			
			actor.SetLastRotationDir(newDir);
			
			actor.SetLerpTimer(0.f);
			
		}
	}
}

void Scene::CreateBullet(bool isEnemy, Entity* entity, btRigidBody* body)
{
	
	if (isEnemy)
	{
		

		if (entity != nullptr)
		{
			glm::vec3 enemyPos = physics->GetBodyPositon(*body);
			glm::vec3 origin = glm::vec3(enemyPos.x, 10.f, enemyPos.z);
			btVector3 btOrigin = btVector3(body->getWorldTransform().getOrigin().x(), 10.f, body->getWorldTransform().getOrigin().z());
			btVector3 target = physics->GetPlayerRigidBody()->getWorldTransform().getOrigin();
			btRigidBody* bulletBox = physics->CreateDefaultBox(btVector3(0.1f, 0.1f, 0.1f), btOrigin, 1.f);
			InitPhysicsCollision(*bulletBox);
			btVector3 direction = physics->NormalizeVector(btOrigin, target);
			btScalar speed = 600.f;
			bulletBox->setLinearVelocity(direction * speed);

			Entity* bulletE = new Entity("../Common/Assets/Items/Bullet/9x19mm.obj");
			room->AddChild(bulletE);
			bulletE->transform->position = origin;
			bulletE->transform->rotation.x = 90.f;
			bulletE->transform->scale = glm::vec3(3.f, 3.f, 3.f);

			BulletStruct bullet;
			bullet.body = bulletBox;
			bullet.entity = bulletE;
			bullet.isPlayer = false;
			bullets.push_back(bullet);
			
		}
	}
	else
	{
		btVector3 playerBox = physics->GetbtBodyPosition(*body);
		glm::mat4 GunWTransform = gun->transform->GetGlobalModelMatrix();
		glm::vec4 GunWPos = glm::vec4(GunWTransform * glm::vec4(gun->transform->position, 1.f));
		
		btVector3 btOrigin = btVector3(playerBox.x(), 15.f, playerBox.z() - 3.f);
		
		btVector3 direction = btVector3(camera->Front.x, camera->Front.y, camera->Front.z);
		btRigidBody* bulletBox = physics->CreateDefaultBox(btVector3(0.1f, 0.1f, 0.1f), btOrigin, 1.f);
		InitPhysicsCollision(*bulletBox);
		
		btScalar speed = 600.f;
		bulletBox->setLinearVelocity(direction * speed);

		Entity* bulletE = new Entity("../Common/Assets/Items/Bullet/9x19mm.obj");
		room->AddChild(bulletE);
		glm::vec3 newOrigin = glm::vec3(btOrigin.x(), btOrigin.y(), btOrigin.z());
		bulletE->transform->position = newOrigin;
		bulletE->transform->rotation.x = 90.f;
		bulletE->transform->scale = glm::vec3(3.f, 3.f, 3.f);

		BulletStruct bullet;
		bullet.body = bulletBox;
		bullet.entity = bulletE;
		bullet.isPlayer = true;
		bullets.push_back(bullet);
	}
}



void Scene::DestroyEnemy()
{
	for (auto it = enemies.begin(); it != enemies.end(); ) {
		if (it->entity != nullptr) {
			if (!it->entity->GetIsAlive()) {
				room->RemoveUnusedChildren(it->entity);
				delete it->entity;         // Free the memory for the entity
				it->entity = nullptr;

				physics->DeleteRigidBody(it->body); // Delete the rigid body
				it->body = nullptr;
				delete it->animator;
				it->animator = nullptr;

				// Erase the current element and update the iterator
				it = enemies.erase(it);
				continue; // Skip incrementing the iterator manually
			}
		}
		++it; // Increment iterator if no deletion
	}
}




void Scene::InitPhysicsCollision(btRigidBody& body)
{
	body.setSleepingThresholds(0, 0);
	body.setAngularFactor(btVector3(0, 0, 0));
	
	body.setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE); 

	
	
}

void Scene::CreatePathBox(btVector3 position)
{
	btRigidBody* box = physics->CreateDefaultBox(btVector3(0.1f, 6.f, 0.1f), position, 0.f);
	InitPhysicsCollision(*box);
	pathBoxes.push_back(box);
}

void Scene::CreateEnemy(btVector3 position)
{
	Enemy* enemy = new Enemy();
	enemy->transform->scale = glm::vec3(5.f, 5.f, 5.f);
	room->AddChild(enemy);
	btRigidBody* enemyBox = physics->CreateBox(CollisionTypes::ENEMY, CollisionTypes::WALL , CollisionTypes::PLAYER, btVector3(3.f, 8.f, 3.f), position, 1.f);
	InitPhysicsCollision(*enemyBox);

	Animation* animator = new Animation();
	animator->LoadAnimation("../Common/Assets/Animations/Enemy/Walking", 42);
	enemy->AddChild(animator->GetModelFrame()[animator->GetFrame()]);
	for (Entity* entity : animator->GetModelFrame())
	{
		entity->transform->scale = glm::vec3(1.f, 1.f, 1.f);
	}

	EnemyStruct enemyS;
	enemyS.entity = enemy;
	enemyS.body = enemyBox;
	enemyS.isAlert = false;
	enemyS.timer = 0.f;
	enemyS.animator = animator;
	enemies.push_back(enemyS);
}

void Scene::ShootPlayerGun()
{
	if (gun->GetCanShoot() && !gun->GetIsMagazineEmpty())
	{
		CreateBullet(false, player, physics->GetPlayerRigidBody());
		gun->BulletOut();
		gun->SetCanShoot(false);
		isFire = true;
		/*for (EnemyStruct& enemy : enemies)
		{
			glm::vec3 nFront = glm::normalize(camera->Front);
			btVector3 cameraFront = btVector3(nFront.x, nFront.y, nFront.z);
			btVector3 target = physics->GetbtBodyPosition(*physics->GetPlayerRigidBody()) + cameraFront * 100.f;
			
			if (physics->PerformRaycast(physics->GetbtBodyPosition(*physics->GetPlayerRigidBody()), target, btVector3(0.f, 1.f, 0.f), ENEMY))
			{
				enemy.entity->ReceiveDamage();
			}
			
			
		}*/
	}
	
}

void Scene::SetIsChangingRoom(bool value)
{
	isChangingRoom = value;
}

void Scene::SetDidUserWin(bool value)
{
	didUserWin = value;
}
