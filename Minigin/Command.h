#pragma once
#include <iostream>
#include "GameObject.h"
#include "Subject.h"
#include "AudioLocator.h"
#include "AudioBase.h"
#include "AudioManager.h"

namespace kaas
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute(GameObject* gameObject) = 0;
	};

	class EmptyCommand : public Command
	{
	public:
		void Execute(GameObject* gameObject) override { UNREFERENCED_PARAMETER(gameObject); }
	};

	class LifeCommand : public Command
	{
	public:
		void Execute(GameObject* gameObject) override {
			gameObject->GetSubject()->notify(*gameObject, Event::LoseLife);
		}
	};

	class SoundCommand : public Command
	{
	public:
		void Execute(GameObject* gameObject) override { 
			AudioManager* manager = static_cast<AudioManager*>(AudioLocator::getAudio());
			manager->AddRequest(0, 19, 0);
			//AudioLocator::getAudio()->PlaySound(0, 19, 0);
			UNREFERENCED_PARAMETER(gameObject); 
		}
	};

	class FireCommand : public Command
	{
	public:
		void Execute(GameObject* gameObject) override {
			AudioManager* manager = static_cast<AudioManager*>(AudioLocator::getAudio());
			manager->AddRequest(1, 19, 0);
			//AudioLocator::getAudio()->PlaySound(0, 19, 0);
			UNREFERENCED_PARAMETER(gameObject);
		}
	};

	class ShieldCommand : public Command
	{
	public:
		void Execute(GameObject* gameObject) override { UNREFERENCED_PARAMETER(gameObject); }
	};
}