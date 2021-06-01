#pragma once
#include <winnt.h>

namespace kaas 
{
	class AudioBase
	{
	public:
		virtual ~AudioBase() = default;
		virtual void PlaySound(int soundID, int volume = 128, int loops = -1) = 0;
		virtual void StopSound(int soundID) = 0;
		virtual void AddSound(std::string soundFilePath) = 0;
		virtual void Update() = 0;
	};
}