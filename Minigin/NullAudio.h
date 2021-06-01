#pragma once
#include "AudioBase.h"

namespace kaas 
{
	class NullAudio : public AudioBase
	{
	public:
		void PlaySound(int soundID, int volume = 128, int loops = -1) override { UNREFERENCED_PARAMETER(soundID); UNREFERENCED_PARAMETER(volume);
		UNREFERENCED_PARAMETER(loops);
		}
		void StopSound(int soundID) override { UNREFERENCED_PARAMETER(soundID); }
		void AddSound(std::string soundFilePath) override { UNREFERENCED_PARAMETER(soundFilePath); }
		void Update() override {};
	};
}