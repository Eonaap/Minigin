#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "TextureComponent.h"
#include "TransformComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "Scene.h"
#include "Time.h"
#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

using namespace std;
using namespace std::chrono;

void kaas::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void kaas::Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto go = new GameObject{};
	TextureComponent* textureComp = new TextureComponent{ go, "background.jpg" };
	go->AddComponent(textureComp);
	scene.Add(go);

	//go = std::make_shared<GameObject>();
	//go->SetTexture("logo.png");
	//go->SetPosition(216, 180);
	//scene.Add(go);
	auto go1 = new GameObject{};
	TextureComponent* textureComp1 = new TextureComponent{ go1, "logo.png" };
	glm::vec2 pos{};
	pos.x = 216.0f;
	pos.y = 180.0f;
	TransformComponent* transComp = new TransformComponent{ go1, pos };
	go1->AddComponent(textureComp1);
	go1->AddComponent(transComp);
	scene.Add(go1);

	auto go2 = new GameObject{};
	auto pFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	TextComponent* textComp = new TextComponent{ go2, "Programming 4 Assignment", pFont };
	pos.x = 80.0f;
	pos.y = 20.0f;
	TransformComponent* transComp1 = new TransformComponent{ go2, pos };
	go2->AddComponent(transComp1);
	go2->AddComponent(textComp);
	scene.Add(go2);

	auto go3 = new GameObject{};
	auto pFont1 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	TextComponent* textComp1 = new TextComponent{ go3, " ", pFont1 };
	pos.x = 0.0f;
	pos.y = 0.0f;
	TransformComponent* transComp2 = new TransformComponent{ go3, pos };
	FPSComponent* FPSComp = new FPSComponent{go3};
	go3->AddComponent(transComp2);
	go3->AddComponent(textComp1);
	go3->AddComponent(FPSComp);
	scene.Add(go3);
}

void kaas::Minigin::Cleanup()
{


	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void kaas::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();

		bool doContinue = true;
		auto lastTime = high_resolution_clock::now();

		while (doContinue)
		{
			const auto currentTime = high_resolution_clock::now();
			Time::GetInstance().SetDeltaTime(duration<float>(currentTime - lastTime).count());
			lastTime = currentTime;
			
			doContinue = input.ProcessInput();
			sceneManager.Update();
			renderer.Render();
		}
	}

	Cleanup();
}
