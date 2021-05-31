#include "MiniginPCH.h"
#include "AudioManager.h"

kaas::AudioManager::AudioManager()
{
	// Initialize Simple-SDL2-Audio
	initAudio();

	//Load music
	m_pMusicTest = Mix_LoadMUS("../Data/Music.wav");
	if (m_pMusicTest == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}
}

kaas::AudioManager::~AudioManager()
{
	endAudio();

	//Free the music
	Mix_FreeMusic(m_pMusicTest);
	m_pMusicTest = NULL;
}

void kaas::AudioManager::PlaySound(int soundID)
{
	// Play sound using console audio api...
	UNREFERENCED_PARAMETER(soundID);
	//playSound(m_AudioPaths[soundID].c_str(), SDL_MIX_MAXVOLUME);
	playSoundFromMemory(m_AudioList[soundID], 128);

	//Play the music
	Mix_PlayMusic(m_pMusicTest, 0);
}

void kaas::AudioManager::StopSound(int soundID)
{
	// Stop sound using console audio api...
	UNREFERENCED_PARAMETER(soundID);
}

void kaas::AudioManager::StopAllSounds()
{
	// Stop all sounds using console audio api...
}

void kaas::AudioManager::AddSound(std::string soundFilePath)
{
	//m_AudioPaths.emplace_back(std::move(soundFilePath));
	m_AudioList.push_back(createAudio(soundFilePath.c_str(), 1, 128));
}
