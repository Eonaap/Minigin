#pragma once
#include "AudioBase.h"
#include <vector>
#include <string>
#include "../3rdParty/SDL2_mixer/audio.h"
#include <SDL_mixer.h>

struct AudioPiece
{
	Mix_Chunk* audio;
	int channel;
	bool playing;
};

struct PlayMessage
{
	int soundID = 0;
	int volume = 0;
	int loops = 0;
};

namespace kaas 
{
	class AudioManager : public AudioBase
	{
	public:
		AudioManager();
		~AudioManager();

		void PlaySound(int soundID, int volume = 128, int loops = -1) override;

		void StopSound(int soundID) override;

		void AddSound(std::string soundFilePath) override;

		void Update() override;

		void AddRequest(int soundID, int volume = 128, int loops = -1);

	private:
		std::vector<AudioPiece> m_pAudioList;

		static int m_Head;
		static int m_Tail;
		static int m_NumPending;
		static const int m_MaxRequests = 16;
		static std::vector<PlayMessage> m_PendingList;
	};
}