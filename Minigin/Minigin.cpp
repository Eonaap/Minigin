#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include <SDL.h>
#include <SDL_mixer.h>
#include "Renderer.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Timer.h"
#include "AudioLocator.h"

using namespace std;
using namespace std::chrono;

void kaas::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	
	//Initialize SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 4, 2048) < 0)
		std::cout << (std::string("SDL_Mixer Error: ") + Mix_GetError());

	// allocate 16 mixing channels
	//Mix_AllocateChannels(16);

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

void kaas::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	AudioLocator::Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	Mix_Quit();
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
			Timer::GetInstance().SetDeltaTime(duration<float>(currentTime - lastTime).count());
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
