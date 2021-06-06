#include "MiniginPCH.h"
#include "InputComponent.h"
#include <SDL.h>
#include "Command.h"
#include "InputManager.h"

kaas::InputComponent::InputComponent(GameObject* pGameObject)
	:BaseComponent{pGameObject}
{
}

void kaas::InputComponent::Update()
{
	for (ControllerAction& action: m_Actions)
	{
		if (action.state == PressingState::ThumbStick )
		{
			UpdateThumbStick(action);
		}
		else if(InputManager::GetInstance().ProcessControllerButton(action) || InputManager::GetInstance().KeyIsPressed(action.keyboardButton))
		{
			action.command->Execute(m_pGameObject);
		}
	}
}

void kaas::InputComponent::UpdateThumbStick(ControllerAction& action)
{
	//Move to inputComponent
	XINPUT_STATE currentState = InputManager::GetInstance().GetCurrentState();
	float RX = currentState.Gamepad.sThumbRX;
	float RY = currentState.Gamepad.sThumbRY;
	float magnitude = sqrt(RX * RX + RY * RY);

	if (magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE * 3)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE * 3;

		//Return true
		action.command->Execute(m_pGameObject, glm::vec2{ RX, RY });
	}
}

void kaas::InputComponent::Render() const
{
}

void kaas::InputComponent::AddAction(ControllerAction action)
{
	m_Actions.push_back(action);
}
