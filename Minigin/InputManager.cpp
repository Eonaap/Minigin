#include "InputManager.h"
#include <iostream>
#include <string>

kaas::InputManager::InputManager()
{
	m_actions.push_back(ControllerAction{ ControllerButton::ButtonA, new JumpCommand{}, PressingState::buttonUp, false });
	m_actions.push_back(ControllerAction{ ControllerButton::ButtonB, new DuckCommand{}, PressingState::buttonDown, false });
	m_actions.push_back(ControllerAction{ ControllerButton::ButtonX, new FireCommand{}, PressingState::buttonUp, false });
	m_actions.push_back(ControllerAction{ ControllerButton::ButtonY, new ShieldCommand{}, PressingState::buttonPressed, false });
}

kaas::InputManager::~InputManager()
{
	for (ControllerAction action : m_actions)
	{
		delete action.command;
		action.command = nullptr;
	}
}

void kaas::InputManager::ProcessInput()
{
	ZeroMemory(&m_pCurrentState, sizeof(XINPUT_STATE));
	DWORD InputState = XInputGetState(0, &m_pCurrentState);
	if (InputState == ERROR_NOT_CONNECTED)
		std::cout << "No controller found.\n";
	else if (InputState != ERROR_SUCCESS)
		std::cout << "Something went wrong with the input.\n";

	for (ControllerAction& action : m_actions)
	{
		if (IsPressed(action.button))
		{
			if (CheckPressingState(action)) {
				action.command->Execute();
			}

			action.isDown = true;
		}
		else
		{
			if (action.state == PressingState::buttonUp && action.isDown)
				action.command->Execute();

			action.isDown = false;
		}
	}
}

bool kaas::InputManager::IsPressed(ControllerButton button) const
{
	switch (button)
	{
	case kaas::ControllerButton::ButtonA:
		if (m_pCurrentState.Gamepad.wButtons == XINPUT_GAMEPAD_A)
			return true;
		break;
	case kaas::ControllerButton::ButtonB:
		if (m_pCurrentState.Gamepad.wButtons == XINPUT_GAMEPAD_B)
			return true;
		break;
	case kaas::ControllerButton::ButtonX:
		if (m_pCurrentState.Gamepad.wButtons == XINPUT_GAMEPAD_X)
			return true;
		break;
	case kaas::ControllerButton::ButtonY:
		if (m_pCurrentState.Gamepad.wButtons == XINPUT_GAMEPAD_Y)
			return true;
		break;
	}
	return false;
}

bool kaas::InputManager::CheckPressingState(ControllerAction& button)
{
	switch (button.state)
	{
	case kaas::PressingState::buttonPressed:
		return true;
		break;
	case kaas::PressingState::buttonDown:
		if (!button.isDown)
		{
			return true;
		}
		break;
	}
	return false;
}