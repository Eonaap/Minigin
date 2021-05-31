#pragma once
#include "NullAudio.h"

namespace kaas 
{
	class AudioBase;
	class AudioLocator
	{
	public:
		static AudioBase* getAudio();

		static void provide(AudioBase* service);

	private:
		static kaas::AudioBase* pService;
	};
}