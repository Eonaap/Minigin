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
#include "Subject.h"
#include "HealthComponent.h"
#include "LevelComponent.h"
#include "CharacterControllerComponent.h"
#include "AudioLocator.h"
#include "AudioManager.h"
#include <SDL_mixer.h>
#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

using namespace std;
using namespace std::chrono;

void kaas::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	
	//Initialize SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 4, 2048) < 0)
		std::cout << (std::string("SDL_Mixer Error: ") + Mix_GetError());
	
	// == Initialize SDL_Mixer == 
	const int mixerFlags{ MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG };
	if ((Mix_Init(mixerFlags) & mixerFlags) != mixerFlags)
		std::cout << (std::string("SDL_Mixer init Error: ") + Mix_GetError() + '\n');

	// allocate 16 mixing channels
	Mix_AllocateChannels(16);

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

	auto go2 = new GameObject{};
	auto pFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	TextComponent* textComp = new TextComponent{ go2, "Programming 4 Assignment", pFont };
	glm::vec2 pos{};
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
	FPSComponent* FPSComp = new FPSComponent{ go3 };
	go3->AddComponent(transComp2);
	go3->AddComponent(textComp1);
	go3->AddComponent(FPSComp);
	scene.Add(go3);


	//Create the level object
	auto LevelObject = new GameObject{};
	auto pFont2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	pos.x = 25.0f;
	pos.y = 50.0f;
	TransformComponent* transComp3 = new TransformComponent{ LevelObject, pos };
	LevelObject->AddComponent(transComp3);
	TextComponent* pLevelTextComponent = new TextComponent{LevelObject, "Level 1", pFont2 };
	LevelObject->AddComponent(pLevelTextComponent);
	LevelComponent* pLevelComponent = new LevelComponent{ LevelObject, "../Data/LevelDataScaled.json" , "../Data/Tiles.png", glm::vec2{50, 55}, glm::vec2{275, 70} };
	LevelObject->AddComponent(pLevelComponent);
	scene.Add(LevelObject);

	//Create the player
	auto QBertObject = new GameObject{};
	auto pFont3 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	pos.x = 25.0f;
	pos.y = 100.0f;
	TransformComponent* transComp4 = new TransformComponent{ QBertObject, pos };
	QBertObject->AddComponent(transComp4);
	TextComponent* textComp2 = new TextComponent{ QBertObject, " ", pFont3 };

	CharacterControllerComponent* pCharacterController = new CharacterControllerComponent{ QBertObject, pLevelComponent };
	TextureComponent* pCharacterTextureComp = new TextureComponent{QBertObject, "../Data/QBert.png"};
	QBertObject->AddComponent(pCharacterController);
	QBertObject->AddComponent(textComp2);
	QBertObject->AddComponent(pCharacterTextureComp);
	QBertObject->GetSubject()->AddObserver(new HealthComponent{ 3, QBertObject });

	scene.Add(QBertObject);

	//Put QBert as player one in the inputManager
	InputManager::GetInstance().SetPlayerOne(QBertObject);

	//Create the audioManager in the audioLocator and add 2 sounds
	AudioLocator::provide(new AudioManager());
	AudioLocator::getAudio()->AddSound("../Data/Music.wav");
	AudioLocator::getAudio()->AddSound("../Data/DerpNugget.wav");
}

void kaas::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
	Mix_Quit();
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
			
			//Update the event queue of the audioLocator
			AudioLocator::getAudio()->Update();

			doContinue = input.ProcessInput();
			sceneManager.Update();
			renderer.Render();
		}
	}

	Cleanup();
}
