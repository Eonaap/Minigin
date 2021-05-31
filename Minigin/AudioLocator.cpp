#include "MiniginPCH.h"
#include "AudioLocator.h"
#include "AudioBase.h"

kaas::AudioBase* kaas::AudioLocator::pService = new kaas::NullAudio{};

kaas::AudioBase* kaas::AudioLocator::getAudio()
{
	return pService;
}

void kaas::AudioLocator::provide(AudioBase* service)
{
	if (service == nullptr)
	{
		// Revert to null service.
		pService = new NullAudio;
	}
	else
	{
		pService = service;
	}
}