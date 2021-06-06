#include "MiniginPCH.h"
#include "InputComponent.h"
#include <SDL.h>
#include "Command.h"
#include "InputManager.h"

kaas::InputComponent::InputComponent(GameObject* pGameObject, int playerID)
	:BaseComponent{pGameObject}
	,m_PlayerID{playerID}
{
}

kaas::InputComponent::~InputComponent()
{
	for (ControllerAction action : m_Actions)
	{
		delete action.command;
		action.command = nullptr;
	}
}

void kaas::InputComponent::Update()
{
	for (ControllerAction& action: m_Actions)
	{
		if (action.state == PressingState::ThumbStick )
		{
			UpdateThumbStick(action);
		}
		else if(InputManager::GetInstance().ProcessControllerButton(action, m_PlayerID) || InputManager::GetInstance().KeyIsPressed(action.keyboardButton))
		{
			action.command->Execute(m_pGameObject);
		}
	}
}

void kaas::InputComponent::UpdateThumbStick(ControllerAction& action)
{
	XINPUT_STATE currentState{};
	
	if (m_PlayerID == 1)
	{
		currentState = InputManager::GetInstance().GetCurrentState();
	}
	else
	{
		currentState = InputManager::GetInstance().GetCurrentState();
	}
	
	float X = currentState.Gamepad.sThumbRX;
	float Y = currentState.Gamepad.sThumbRY;
	if (action.button == ControllerButton::LeftThumbStick)
	{
		X = currentState.Gamepad.sThumbLX;
		Y = currentState.Gamepad.sThumbLY;
	}

	float magnitude = sqrt(X * X + Y * Y);
	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 3)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE * 3;

		//Return true
		action.command->Execute(m_pGameObject, glm::vec2{ X, Y });
	}
}

void kaas::InputComponent::Render() const
{
}

void kaas::InputComponent::AddAction(ControllerAction action)
{
	m_Actions.push_back(action);
}

int kaas::InputComponent::GetPlayerID() const
{
	return m_PlayerID;
}
