#include "ImGuiEngine.h"
#include "Gun.h"
#include "Entity.h"
#include "Input.h"
#include "IGraphics.h"
#include "IInput.h"
#include "Player.h"
#include <iostream>
#include "Common.h"
#include <string>

//#include "Game.h"

ImGuiEngine::ImGuiEngine() : isMenu(true), isGameplay(false), isGameover(true), tab("Open Inventory:'TAB'"),gameoverText("Gameover!"), showInventory(false),
healthHud(3), isWindowFocused(false), isDebuggerActive(false), isLeaving(false), isLoadScreen(false), winText("You Win!"), isPhysicsRender(false)
{
	
}

ImGuiEngine::~ImGuiEngine()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiEngine::Init()
{
	printf("This cross project was partly inspired by BUas Student Ferri de Lange\n");
	printf("This GPU supplied by  :%s\n", glGetString(GL_VENDOR));
	printf("This GPU supports GL  :%s\n", glGetString(GL_VERSION));
	printf("This GPU Renders with :%s\n", glGetString(GL_RENDERER));
	printf("This GPU Shaders are  :%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	pistolTexture = Render::LoadTexture("../Common/Assets/Textures/pistol.jpg");
	medicKitTexture = LoadTexture("../Common/Assets/Textures/medicKit.jpg");
	loadScreenTexture = Render::LoadTexture("../Common/Assets/Textures/loadingScreen.jpg");

	// imgui setup setup for ES3.x, use #verion 100 for GLES2.0 
	const char* glsl_version = "#version 100";  //310/320es not available
	ImGui::CreateContext();
	//ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(255, 0, 0, 0);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void ImGuiEngine::UpdateMenu(Input& input)
{
	isWindowFocused = false;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowBgAlpha(0.2f);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	ImGuiWindowFlags window_flags = //ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing
		| ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar;

	static bool open = true;

	// open a new window

	ImGui::Begin("helpinfo ", &open, window_flags);
	ImGui::SetWindowFontScale(3.0f);



	// this can be anything
	ImGui::SetCursorPos(ImVec2(WINDOW_WIDTH / 4 - 50, WINDOW_HEIGHT / 2 - 150));
	//ImGui::SetNextWindowPos(ImVec2(100, 100));
	ImGui::Text("Metal Gear Solid (PI Version)");

	ImGui::SetCursorPos(ImVec2(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 + 110));
	if (ImGui::Button("Exit", ImVec2(400, 100)))
	{
		isLeaving = true;

	}
	
	ImGui::SetCursorPos(ImVec2(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 + 220));
	if (!isPhysicsRender)
	{
		if (ImGui::Checkbox("Physics viewer", &isPhysicsRender))
		{
			isPhysicsRender = true;
		}
	}
	else
	{
		if (ImGui::Checkbox("Physics viewer", &isPhysicsRender))
		{
			isPhysicsRender = false;
		}
	}
	ImGui::SetCursorPos(ImVec2(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2));
	if (ImGui::Button("Play", ImVec2(400, 100)))
	{
		isMenu = false;
		ImGui::SetCursorPos(ImVec2(0.f, 0.f));
		ImGui::Image(loadScreenTexture, ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT));
		isWindowFocused = true;
	}
	ImGui::GetIO().AddMousePosEvent(input.GetMouse().GetPosition(true).x, input.GetMouse().GetPosition(true).y);
	ImGui::GetIO().AddMouseButtonEvent(0, input.GetMouse().GetButtonDown(MouseButtons::LEFT));

	//ImGui::SliderFloat(" Rotation", &player.GetRotationRef(), 0.f, 100.f);
	ImGui::End();
}

void ImGuiEngine::UpdateGameplay(Input& input, Player& player, Entity& playerCollider, Gun& gun)
{
		isWindowFocused = true;
		isGameover = true;
		isMenu = true;
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		//ImGui::SetNextWindowBgAlpha(0.2f);
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT));
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing
			| ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMouseInputs;

		static bool open = true;

		// open a new window

		ImGui::Begin("gameplay ", &open, window_flags);
		ImGui::SetWindowFontScale(3.0f);

		// this can be anything
		ImGui::SetCursorPos(ImVec2(50, WINDOW_HEIGHT - 50));
		ImGui::Text("Health: %i", player.GetHealth());

		ImGui::SetWindowFontScale(2.0f);
		ImVec2 tabString = ImGui::CalcTextSize(tab.c_str());
		ImGui::SetCursorPos(ImVec2((WINDOW_WIDTH / 2.f) - (tabString.x / 2.f), WINDOW_HEIGHT - 50));
		ImGui::Text(tab.c_str());

		ImGui::SetWindowFontScale(3.0f);
		ImGui::SetCursorPos(ImVec2(WINDOW_WIDTH - 200, WINDOW_HEIGHT - 50));
		ImGui::Text("%i/%i", gun.GetRemainingBullets(), gun.GetTotalBullets());

		ImGui::GetIO().AddMousePosEvent(input.GetMouse().GetPosition(true).x, input.GetMouse().GetPosition(true).y);
		ImGui::GetIO().AddMouseButtonEvent(0, input.GetMouse().GetButtonDown(MouseButtons::LEFT));
		ImGui::End();

		if (showInventory)
		{
			UpdateInventory(input, player); // Activate Inventory;
		}
		if (isDebuggerActive)
		{
			DebuggerWindow(input, player, playerCollider);
		}
	
}

void ImGuiEngine::UpdateInventory(Input& input, Player& player)
{
	isWindowFocused = false;

	ImGui::SetNextWindowBgAlpha(1.f);
	ImVec2 windowSize = ImVec2(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
	ImGui::SetNextWindowSize(windowSize);
	
	ImGui::SetNextWindowPos(ImVec2((WINDOW_WIDTH - windowSize.x) / 2.f, (WINDOW_HEIGHT - windowSize.y) / 2.f));
	ImGuiWindowFlags window_flags = //ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_AlwaysAutoResize //| ImGuiWindowFlags_NoFocusOnAppearing
		| ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar;

	static bool open = true;

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.5f, 0.0f, 0.0f, 1.0f));

	// open a new window

	ImGui::Begin("inventory ", &open, window_flags);
	ImGui::SetWindowFontScale(5.0f);


	ImGui::GetIO().AddMousePosEvent(input.GetMouse().GetPosition(true).x, input.GetMouse().GetPosition(true).y);
	ImGui::GetIO().AddMouseButtonEvent(0, input.GetMouse().GetButtonDown(MouseButtons::LEFT));

	// this can be anything
	//std::cout << myTexture << std::endl;
	if (ImGui::ImageButton("Pistol",pistolTexture, ImVec2(100, 100)))
	{
		std::cout << "AAA" << std::endl;
	}
	ImGui::SetCursorPos(ImVec2(130, 10));

	if (ImGui::ImageButton("MedicKit", medicKitTexture, ImVec2(100, 100)))
	{
		healthHud += 1;
		
		player.IncreaseHealth();
	}

	//ImGui::SliderFloat(" Rotation", &player.GetRotationRef(), 0.f, 100.f);
	ImGui::End();
	ImGui::PopStyleColor();
}

void ImGuiEngine::UpdateGameover(Input& input)
{
	isWindowFocused = false;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowBgAlpha(0.2f);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	ImGuiWindowFlags window_flags = //ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing
		| ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar;

	static bool open = true;

	// open a new window

	ImGui::Begin("helpinfo ", &open, window_flags);
	ImGui::SetWindowFontScale(5.0f);



	// this can be anything
	ImVec2 overString = ImGui::CalcTextSize(gameoverText.c_str());
	ImGui::SetCursorPos(ImVec2((WINDOW_WIDTH / 2.f) - (overString.x / 2.f), WINDOW_HEIGHT / 2.f - 150));
	ImGui::Text(gameoverText.c_str());

	ImGui::SetWindowFontScale(3.0f);
	ImGui::SetCursorPos(ImVec2(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2));
	if (ImGui::Button("Retry", ImVec2(400, 100)))
	{
		isGameover = false;
		
	}
	
	ImGui::GetIO().AddMousePosEvent(input.GetMouse().GetPosition(true).x, input.GetMouse().GetPosition(true).y);
	ImGui::GetIO().AddMouseButtonEvent(0, input.GetMouse().GetButtonDown(MouseButtons::LEFT));

	ImGui::End();
}

void ImGuiEngine::UpdateWinScreen(Input& input)
{
	isWindowFocused = false;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowBgAlpha(0.2f);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	ImGuiWindowFlags window_flags = //ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing
		| ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar;

	static bool open = true;

	// open a new window

	ImGui::Begin("helpinfo ", &open, window_flags);
	ImGui::SetWindowFontScale(5.0f);



	// this can be anything
	ImVec2 winString = ImGui::CalcTextSize(winText.c_str());
	ImGui::SetCursorPos(ImVec2((WINDOW_WIDTH / 2.f) - (winString.x / 2.f), WINDOW_HEIGHT / 2.f - 150));
	ImGui::Text(winText.c_str());

	

	ImGui::GetIO().AddMousePosEvent(input.GetMouse().GetPosition(true).x, input.GetMouse().GetPosition(true).y);
	ImGui::GetIO().AddMouseButtonEvent(0, input.GetMouse().GetButtonDown(MouseButtons::LEFT));

	ImGui::End();
}

void ImGuiEngine::UpdateLoadScreen()
{
	isWindowFocused = false;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowBgAlpha(0.2f);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar
		|// ImGuiWindowFlags_AlwaysAutoResize |
		// ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar;

	static bool open = true;

	// open a new window

	ImGui::Begin("helpinfo ", &open, window_flags);
	ImGui::SetWindowFontScale(3.0f);



	// this can be anything
	

	ImGui::End();
	
}

void ImGuiEngine::DebuggerWindow(Input& input, Player& player, Entity& playerCollider)
{
	isWindowFocused = false;

	ImGui::SetNextWindowBgAlpha(1.f);
	ImVec2 windowSize = ImVec2(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 3.f);
	ImGui::SetNextWindowSize(windowSize);

	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
	ImGuiWindowFlags window_flags = //ImGuiWindowFlags_NoBackground |
		//ImGuiWindowFlags_NoTitleBar
		 ImGuiWindowFlags_AlwaysAutoResize //| ImGuiWindowFlags_NoFocusOnAppearing
		| ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar;

	static bool open = true;

	//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.5f, 0.0f, 0.0f, 1.0f));

	// open a new window

	ImGui::Begin("inventory ", &open, window_flags);
	ImGui::SetWindowFontScale(3.0f);


	ImGui::GetIO().AddMousePosEvent(input.GetMouse().GetPosition(true).x, input.GetMouse().GetPosition(true).y);
	ImGui::GetIO().AddMouseButtonEvent(0, input.GetMouse().GetButtonDown(MouseButtons::LEFT));

	// this can be anything
	//std::cout << myTexture << std::endl;
	/*if (ImGui::ImageButton("Pistol", pistolTexture, ImVec2(100, 100)))
	{
		std::cout << "AAA" << std::endl;
	}
	ImGui::SetCursorPos(ImVec2(130, 10));

	if (ImGui::ImageButton("MedicKit", medicKitTexture, ImVec2(100, 100)))
	{
		healthHud += 1;

		player.IncreaseHealth();
	}*/

	//ImGui::SliderFloat(" Rotation", &player.transform->rotation.y, 0.f, 100.f);
	glm::mat4 colliderWorldTransform = playerCollider.transform->modelMatrix;
	glm::vec4 playerWorldPos = colliderWorldTransform * glm::vec4(player.transform->position, 1.f);
	;
	//std::cout << playerWorldPos.x << " / " << playerWorldPos.y << " / " << playerWorldPos.z << std::endl;
	ImGui::SetWindowFontScale(2.0f);
	ImGui::Text("Player World Space Position:");
	ImGui::SetWindowFontScale(3.0f);
	ImGui::Text("X: %f", playerWorldPos.x );
	ImGui::Text("Y: %f", playerWorldPos.y);
	ImGui::Text("Z: %f", playerWorldPos.z);
	ImGui::End();
	//ImGui::PopStyleColor();
}

void ImGuiEngine::ChangingRoomWindow(bool value)
{
	if (value)
	{
		isWindowFocused = false;

		ImGui::SetNextWindowBgAlpha(1.f);
		ImVec2 windowSize = ImVec2(WINDOW_WIDTH , WINDOW_HEIGHT );
		ImGui::SetNextWindowSize(windowSize);

		ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
		ImGuiWindowFlags window_flags = //ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_AlwaysAutoResize //| ImGuiWindowFlags_NoFocusOnAppearing
			| ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar;

		static bool open = true;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, 0.0f, 0.0f, 1.0f));
		

		ImGui::Begin("ChangingRoom ", &open, window_flags);
		ImGui::SetWindowFontScale(3.0f);

	
		ImGui::End();
		ImGui::PopStyleColor();
	}
	
}


void ImGuiEngine::Draw()
{
	
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (isLoadScreen)
		{
			isLoadScreen = false;
		}

	
	
	
}



void ImGuiEngine::SetShowInventory(bool value)
{

	showInventory = value;
}

void ImGuiEngine::SetIsWindowFocused(bool value)
{
	isWindowFocused = value;
}

bool ImGuiEngine::GetIsPhysicsRender() const
{
	return isPhysicsRender;
}

bool ImGuiEngine::GetIsMenu() const
{
	return isMenu;
}

bool ImGuiEngine::GetIsLoadScreen() const
{
	return isLoadScreen;
}

bool ImGuiEngine::GetIsGameover() const
{
	return isGameover;
}

bool ImGuiEngine::GetShowInventory() const
{
	return showInventory;
}

bool ImGuiEngine::GetWindowFocus() const
{
	return isWindowFocused;
}

bool ImGuiEngine::GetIsLeaving() const
{
	return isLeaving;
}
