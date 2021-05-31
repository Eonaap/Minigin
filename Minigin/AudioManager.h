#pragma once
#include "AudioBase.h"
#include <vector>
#include <string>
#include "../3rdParty/SDL2_mixer/audio.h"
#include <SDL_mixer.h>

namespace kaas 
{
	class AudioManager : public AudioBase
	{
	public:
		AudioManager();
		~AudioManager();

		void PlaySound(int soundID) override;

		void StopSound(int soundID) override;

		void StopAllSounds() override;

		void AddSound(std::string soundFilePath) override;

	private:
		std::vector<std::string> m_AudioPaths;
		std::vector<Audio*> m_AudioList;
		Mix_Music* m_pMusicTest;
	};
}