#pragma once


#ifdef WINDOWS_BUILD
#include <GLFW/glfw3.h>
#else
#include <GLES2/gl2.h>
#endif

#ifdef Raspberry_BUILD
#include <GLES3/gl3.h>
#include <GLES2/gl2.h>
#endif

#ifdef WINDOWS_BUILD
#include "glad/glad.h"

#endif
#include "Common.h"

class IGraphics;
class Input;
class Shader;
class Camera;
class Model;
class Scene;
class Player;
class ImGuiEngine;
class Physics;
class Render;
class SceneGraph;
class Entity;
class NoiseGenerator;

class Game
{
public:
	enum GameState
	{
		MENU,
		LOAD_SCREEN,
		GAMEPLAY,
		GAME_OVER
	}gState;

	Game( Input* const input, IGraphics* graphics);
	~Game();
	void Draw();
	void Start();
	void Update();

	void Init();
	void Quit();
	GameState GetGameState();
	void RaspMouseCallback();

private:
	 Input* const input;
	IGraphics* graphics;

	
	Render* render = nullptr;
	ImGuiEngine* imGui = nullptr;
	Scene* scene = nullptr;
	//Physics* physics = nullptr;
	NoiseGenerator* noise;
	unsigned int noiseId;
	unsigned int imageId;
	

	bool quitting{ false };
	float gameDeltaTime{ 0.0f };
	float gameTime{ 0.0f };

	void ProcessInput();
	bool isTimeInit;

	//GameState
	bool cameraSet;
	bool isMenuSetup;
	bool isGameplaySetup;
	bool isGameOverSetup;

	float inventoryTimer;
	float swappingCamTimer;

	bool firstMouse = true;
	float lastX = WINDOW_WIDTH / 2.f;
	float lastY = WINDOW_HEIGHT / 2.f;
	float playerGunTimer;
	bool playerWon;

};

