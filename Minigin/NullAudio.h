#pragma once
#include "AudioBase.h"

namespace kaas 
{
	class NullAudio : public AudioBase
	{
	public:
		void PlaySound(int soundID) override { UNREFERENCED_PARAMETER(soundID); }
		void StopSound(int soundID) override { UNREFERENCED_PARAMETER(soundID); }
		void StopAllSounds() override {}
		void AddSound(std::string soundFilePath) override { UNREFERENCED_PARAMETER(soundFilePath); }
	};
}