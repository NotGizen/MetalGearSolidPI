#include "Game.h"

#ifdef WINDOWS_BUILD
#include <vld.h>
#endif	

#include "Gun.h"
#include "ImGuiEngine.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Player.h"
#include "Actor.h"
#include "SceneRenderer.h"
#include "Physics.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <math.h>
#include "Render.h"
#include "NoiseGenerator.h"

#include "Input.h"
#include "IGraphics.h"
#include "IInput.h"

#include "Entity.h"


//Test
#include <bullet/btBulletDynamicsCommon.h>
#include <stdio.h>


Game::Game( Input* const input, IGraphics* graphics) : input(input), graphics(graphics), playerWon(false),
isMenuSetup(false), isGameplaySetup(false), isGameOverSetup(false), isTimeInit(false), inventoryTimer(0.f), swappingCamTimer(0.f), playerGunTimer(0.f)
{
	
	render = new Render();
	gState = MENU; //Change Game State Here
	imGui = new ImGuiEngine();
	if (gState == MENU)
	{
		isMenuSetup = true;
	
	}
	//if (gState == GAMEPLAY)
	//{
	//	scene = new Scene();
	//	//physics = new Physics(scene->GetPlayer(), scene->GetRoom().GetModel());
	//	//render = new Render();
	//	isGameplaySetup = true;
	//}

}

Game::~Game()
{
	
	delete scene;
	scene = nullptr;

	delete imGui;
	delete render;

	delete noise;
	
}

void Game::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (gState)
	{

	case Game::GAMEPLAY:
		
		render->RenderScene(scene->GetCamera(), scene->GetLights(), gameDeltaTime, *scene);
		//----------This should be scenegraph rendering---------------//
		//scene->Draw(render->GetSceneShader(), *render, gameDeltaTime);
		render->RenderPerlinNoise(imageId, noiseId, scene->GetCamera());
		if (imGui->GetIsPhysicsRender())
		{
			render->DrawPhysics(*scene);

		}

		break;
	}
	imGui->Draw();
	
	graphics->SwapBuffer();
}


void Game::Start()
{
	
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(0.f, 0.f, 0.f, 1.f);

	auto startTime = std::chrono::system_clock::now();
	auto lastTime = startTime;
	
	
	//----------------INIT-------------//s
	imGui->Init();
	//PERLINNOISE TESTHERE
	imageId = render->LoadTexture("../Common/Assets/Textures/Water.png");
	std::vector<float>noiseData = noise->generatePerlinNoiseTexture(768, 550);
	noiseId = render->createPerlinNoiseTexture(noiseData, 200, 200);
	//---------------------------//
	while(!quitting)
	{
		//---------DeltaTime Update
		
		auto time = std::chrono::system_clock::now();
		std::chrono::duration<float> delta = time - lastTime;
		std::chrono::duration<float> elapsed = time - startTime;
		gameDeltaTime = delta.count();
		gameTime = elapsed.count();
		//Exit
		if (imGui->GetIsLeaving()) Quit();
		
		//-----------UPDATE-----------//
		Update();
		
		//----------RENDER---------------------//
		
		Draw();
		

		//		ImGui::EndFrame(); // actuall this is closed by the render
//*********************//******************************//

		
		lastTime = time;
	}

	graphics->Quit();
}



void Game::Update()
{
#ifdef Raspberry_BUILD
	RaspMouseCallback();
#endif
	ProcessInput();
	if (!imGui->GetIsMenu())
	{
		gState = GAMEPLAY;
	}
	if (!imGui->GetIsGameover())
	{
		gState = GAMEPLAY;
	}

	
	switch (gState)
	{
	case Game::MENU:
		graphics->SetIsWindowsFocused(imGui->GetWindowFocus());
		imGui->UpdateMenu(*input);
		break;
	case Game::GAMEPLAY:
		graphics->SetIsWindowsFocused(imGui->GetWindowFocus());
		
		
		if (!scene )
		{
			noise = new NoiseGenerator();
			scene = new Scene();
			//physics = new Physics(scene->GetPlayer(), scene->GetRoom().GetModel());
			//physics->Init(scene->GetPlayer());
			scene->Init(*render);
		}
		scene->SetDidUserWin(false);
		
		
		if (!cameraSet)
		{
			if (!scene->GetCameraP())
				std::cout << "fAIl" << endl;
			cameraSet = true;
			input->SetCameraReference(scene->GetCameraP());
			
		}
		//TEST PERLIN NOISE
		
		//------------------------//
		//physics->Update(gameDeltaTime, scene->GetPlayer());
		scene->Update(gameDeltaTime);
		imGui->UpdateGameplay(*input, scene->GetPlayer(), scene->GetPlayerCollider(), scene->GetGun());
		imGui->ChangingRoomWindow(scene->GetIsChangingRoom());
		if (scene->GetDidUserWin())
		{
			playerWon = true;
		}
		if (!scene->GetPlayer().GetIsAlive() || playerWon)
		{
			gState = GAME_OVER;
			//delete physics;
			//physics = nullptr;
			delete scene;
			scene = nullptr;
			delete noise;
			noise = nullptr;
		}
		break;
	case Game::GAME_OVER:
		
		imGui->SetIsWindowFocused(false);
		graphics->SetIsWindowsFocused(imGui->GetWindowFocus());
		if (playerWon)
		{
			imGui->UpdateWinScreen(*input);
		}
		else
		{
			imGui->UpdateGameover(*input);

		}
		cameraSet = false;
		//graphics->SetIsWindowsFocused(imGui->GetWindowFocus());
		break;
	default:
		break;
	}
	
	

}

void Game::Init()
{
	

		

}

void Game::Quit()
{
	quitting = true;
}

Game::GameState Game::GetGameState()
{
	return gState;
}

void Game::RaspMouseCallback()
{
	if (Input::camera)
	{
		if (imGui->GetShowInventory()) // Inventory mouse
		{
			float xpos = static_cast<float>(input->GetMouse().GetPosition(true).x);
			float ypos = static_cast<float>(input->GetMouse().GetPosition(true).y);

			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			// Calculate the offsets between the current and last mouse positions
			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos; // Y-axis is reversed in GLFW

			lastX = xpos;
			lastY = ypos;


			Input::camera->ProcessMouseMovement(xoffset, yoffset);

			//graphics->MouseWrap();
		}
		else //Normal view
		{
			// Get current mouse position
			glm::vec2 mousePos = input->GetMouse().GetPosition(false);

			float xpos = mousePos.x;
			float ypos = mousePos.y;

			// Calculate offsets between the current and last mouse positions
			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos; // Y-axis is reversed in X11

			// Update the last mouse position to the center of the window
			lastX = WINDOW_WIDTH / 2;
			lastY = WINDOW_HEIGHT / 2;

			// Pass the relative movement to the camera
			Input::camera->ProcessMouseMovement(xoffset, yoffset);

			// Reset mouse position to the center of the window
			graphics->MouseWrap();

		}
	}
	
}




//example of using the key and mouse

void Game::ProcessInput()
{
	
	const IKeyboard& keyboard = input->GetKeyboard();
	const IMouse& mouse = input->GetMouse();
	if (gState == GAMEPLAY)
	{
	
		Player& player = scene->GetPlayer();
		Camera& cam = scene->GetCamera();
		player.posDir.front = false;
		player.posDir.back = false;
		player.posDir.left = false;
		player.posDir.right = false;
		player.posDir.idle = true;
		//player.dirPos = player.IDLE;

		if (keyboard.GetKey(Key::W))
		{
			//printf("we pressed W\n");
			cam.camDir = cam.FORWARD;
			if (cam.type == Camera::ThirdPerson)
			{
				if (scene->GetLevel() == Scene::LEVEL1)
				{
					cam.SetYaw(180.f);
				}
				else
				{
					cam.SetYaw(0.f);
				}

			}
			player.posDir.front = true;
			player.posDir.idle = false;

		}
		if (keyboard.GetKey(Key::S))
		{
			//printf("we pressed S\n");
			cam.camDir = cam.BACKWARD;
			if (cam.type == Camera::ThirdPerson)
			{
				if (scene->GetLevel() == Scene::LEVEL1)
				{
					cam.SetYaw(0.f);
				}
				else
				{
					cam.SetYaw(180.f);
				}
				

			}
			player.posDir.back = true;
			player.posDir.idle = false;
		}
		if (keyboard.GetKey(Key::A))
		{
			//printf("we pressed A\n");
			cam.camDir = cam.LEFT;
			if (cam.type == Camera::ThirdPerson)
			{
				if (scene->GetLevel() == Scene::LEVEL1)
				{
					cam.SetYaw(90.f);
				}
				else
				{
					cam.SetYaw(270.f);
				}

			}
			player.posDir.left = true;
			player.posDir.idle = false;
		}
		if (keyboard.GetKey(Key::D))
		{
			//printf("we pressed D\n");
			cam.camDir = cam.RIGHT;
			if (cam.type == Camera::ThirdPerson)
			{
				if (scene->GetLevel() == Scene::LEVEL1)
				{
					cam.SetYaw(270.f);
				}
				else
				{
					cam.SetYaw(90.f);
				}

			}
			player.posDir.right = true;
			player.posDir.idle = false;
		}
		if (keyboard.GetKey(Key::K))
		{
			player.SetIsAlive(false);
		}
		if (keyboard.GetKey(Key::L) )
		{
			if (swappingCamTimer >= 1.f)
			{
				if (cam.type == Camera::FirstPerson)
				{
					cam.SetIsTypeChanged(true);
					
					cam.type = Camera::ThirdPerson;
				}
				else
				{
					cam.SetIsTypeChanged(true);
					
					cam.type = Camera::FirstPerson;
				}
				swappingCamTimer = 0.f;
			}
		}
		swappingCamTimer += gameDeltaTime;		
		
		if (keyboard.GetKey(Key::TAB))
		{
			if (inventoryTimer >= 1.f)
			{
				if (imGui->GetShowInventory() == false)
				{
					imGui->SetShowInventory(true);

				}
				else
				{
					imGui->SetShowInventory(false);
				}
				inventoryTimer = 0.f;
			}
				
		}
		inventoryTimer += gameDeltaTime ;
		if (inventoryTimer >= 1.f) inventoryTimer = 1.f;
		
		if (mouse.GetButtonDown(MouseButtons::LEFT))
		{
			if (cam.type == Camera::FirstPerson)
			{
				if (playerGunTimer >= 0.5f)
				{
					Gun& playerGun = scene->GetGun();
					playerGun.SetCanShoot(true);
					playerGunTimer = 0.f;
				}

			}	
		}
		playerGunTimer += gameDeltaTime;
		if (playerGunTimer >= 0.5f) playerGunTimer = 0.5f;
	}
	if (keyboard.GetKey(Key::ESCAPE))
	{
		Quit();
	}
	
	
}
