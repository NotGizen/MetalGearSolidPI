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
#include "ImGui-master/imgui.h"
#include "ImGui-master/backends/imgui_impl_opengl3.h"
#include "Render.h"
#include <string>
class Input;
class Player;
class Entity;
class Gun;
//class Game;

class ImGuiEngine : public Render
{
public:

	ImGuiEngine();
	~ImGuiEngine();
	void Init();
	void UpdateMenu(Input& input);
	void UpdateGameplay(Input& input, Player& player, Entity& playerCollider, Gun& gun);
	void UpdateInventory(Input& input, Player& player);
	void UpdateGameover(Input& input);
	void UpdateWinScreen(Input& input);
	void UpdateLoadScreen();
	void DebuggerWindow(Input& input, Player& player, Entity& playerCollider);
	void ChangingRoomWindow(bool ischangingroom);
	void Draw();
	
	//Set
	void SetShowInventory(bool value);
	void SetIsWindowFocused(bool value);

	//Get
	bool GetIsPhysicsRender() const;
	bool GetIsMenu() const; 
	bool GetIsLoadScreen() const;
	bool GetIsGameover() const;
	bool GetShowInventory() const;
	bool GetWindowFocus() const;
	bool GetIsLeaving() const;
private:
	
	bool isMenu;
	bool isLoadScreen;
	bool isGameplay;
	bool isGameover;
	bool showInventory;
	bool isLeaving;
	std::string tab;
	std::string gameoverText;
	std::string winText;
	unsigned int pistolTexture;
	unsigned int medicKitTexture;
	unsigned int loadScreenTexture;
	int healthHud;
	bool isWindowFocused;
	bool isDebuggerActive;
	bool isPhysicsRender;
	
};

