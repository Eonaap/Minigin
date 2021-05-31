#pragma once
namespace kaas 
{
	class AudioBase
	{
	public:
		virtual ~AudioBase() = default;
		virtual void PlaySound(int soundID) = 0;
		virtual void StopSound(int soundID) = 0;
		virtual void StopAllSounds() = 0;
		virtual void AddSound(std::string soundFilePath) = 0;
	};
}