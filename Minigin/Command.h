#pragma once
#include <iostream>
#include "GameObject.h"
#include "Subject.h"
#include "AudioLocator.h"
#include "AudioBase.h"
#include "AudioManager.h"
#include "CharacterControllerComponent.h"
#include <glm.hpp>

namespace kaas
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute(GameObject* gameObject) = 0;
		virtual void Execute(GameObject* gameObject, glm::vec2 thumbStickValue) = 0;
	};

	//Delete if you don't use it
	class EmptyCommand : public Command
	{
	public:
		void Execute(GameObject* gameObject) override { UNREFERENCED_PARAMETER(gameObject); }
		void Execute(GameObject* gameObject, glm::vec2 thumbStickValue) { UNREFERENCED_PARAMETER(gameObject); UNREFERENCED_PARAMETER(thumbStickValue); };
	};

	class LifeCommand : public Command
	{
	public:
		void Execute(GameObject* gameObject) override {
			gameObject->GetSubject()->notify(*gameObject, Event::LoseLife);
		}

		void Execute(GameObject* gameObject, glm::vec2 thumbStickValue) { UNREFERENCED_PARAMETER(gameObject); UNREFERENCED_PARAMETER(thumbStickValue); };
	};

	class SoundCommand : public Command
	{
	public:
		void Execute(GameObject* gameObject) override { 
			AudioManager* manager = static_cast<AudioManager*>(AudioLocator::getAudio());
			manager->AddRequest(0, 19, 0);
			UNREFERENCED_PARAMETER(gameObject); 
		}

		void Execute(GameObject* gameObject, glm::vec2 thumbStickValue) override { UNREFERENCED_PARAMETER(gameObject); UNREFERENCED_PARAMETER(thumbStickValue); };
	};

	class MoveTopLeftCommand : public Command
	{
	public:
		void Execute(GameObject* gameObject) override {
			CharacterControllerComponent* temp{ gameObject->GetComponent<CharacterControllerComponent>() };
			if (temp)
			{
				temp->SetTarget(int(MovementDirections::topLeft));
			}
		}

		void Execute(GameObject* gameObject, glm::vec2 thumbStickValue) override { UNREFERENCED_PARAMETER(gameObject); UNREFERENCED_PARAMETER(thumbStickValue); };
	};

	class MoveTopRightCommand : public Command
	{
	public:
		void Execute(GameObject* gameObject) override {
			CharacterControllerComponent* temp{ gameObject->GetComponent<CharacterControllerComponent>() };
			if (temp)
			{
				temp->SetTarget(int(MovementDirections::topRight));
			}
		}

		void Execute(GameObject* gameObject, glm::vec2 thumbStickValue) override { UNREFERENCED_PARAMETER(gameObject); UNREFERENCED_PARAMETER(thumbStickValue); };
	};

	class MoveBottomLeftCommand : public Command
	{
	public:
		void Execute(GameObject* gameObject) override {
			CharacterControllerComponent* temp{ gameObject->GetComponent<CharacterControllerComponent>() };
			if (temp)
			{
				temp->SetTarget(int(MovementDirections::topRight));
			}
		}

		void Execute(GameObject* gameObject, glm::vec2 thumbStickValue) override { UNREFERENCED_PARAMETER(gameObject); UNREFERENCED_PARAMETER(thumbStickValue); };
	};

	class MoveBottomRightCommand : public Command
	{
	public:
		void Execute(GameObject* gameObject) override {
			CharacterControllerComponent* temp{ gameObject->GetComponent<CharacterControllerComponent>() };
			if (temp)
			{
				temp->SetTarget(int(MovementDirections::topRight));
			}
		}

		void Execute(GameObject* gameObject, glm::vec2 thumbStickValue) override { UNREFERENCED_PARAMETER(gameObject); UNREFERENCED_PARAMETER(thumbStickValue); };
	};

	class MoveCommand : public Command
	{
	public:
		void Execute(GameObject* gameObject) override { UNREFERENCED_PARAMETER(gameObject); }
		void Execute(GameObject* gameObject, glm::vec2 thumbStickValue) override 
		{
			MovementDirections direction{};

			//Direction is on the right
			if (thumbStickValue.x > 0.0f)
			{
				//Direction is on top
				if (thumbStickValue.y > 0.0f)
				{
					direction = MovementDirections::topRight;
				}
				else
				{
					direction = MovementDirections::bottomRight;
				}
			}
			//Direction is on the left
			else
			{
				//Direction is on top
				if (thumbStickValue.y > 0.0f)
				{
					direction = MovementDirections::topLeft;
				}
				else
				{
					direction = MovementDirections::bottomLeft;
				}
			}

			CharacterControllerComponent* temp{ gameObject->GetComponent<CharacterControllerComponent>() };
			if (temp)
			{
				temp->SetTarget(int(direction));
			}
				
		}
	};
}