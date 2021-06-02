#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include "Command.h"

kaas::InputManager::InputManager()
{
	m_actions.push_back(ControllerAction{ ControllerButton::ButtonA, new LifeCommand{}, PressingState::buttonUp, false });
	m_actions.push_back(ControllerAction{ ControllerButton::ButtonB, new SoundCommand{}, PressingState::buttonDown, false });
	m_actions.push_back(ControllerAction{ ControllerButton::ButtonX, new EmptyCommand{}, PressingState::buttonUp, false });
	m_actions.push_back(ControllerAction{ ControllerButton::ButtonY, new EmptyCommand{}, PressingState::buttonPressed, false });
	m_actions.push_back(ControllerAction{ ControllerButton::DPAD_UP, new EmptyCommand{}, PressingState::buttonPressed, false });
	m_actions.push_back(ControllerAction{ ControllerButton::RightThumbStick, new MoveCommand{}, PressingState::ThumbStick, false });
	m_pPlayerOne = nullptr;
}

kaas::InputManager::~InputManager()
{
	for (ControllerAction action : m_actions)
	{
		delete action.command;
		action.command = nullptr;
	}
}

bool kaas::InputManager::ProcessInput()
{
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	DWORD InputState = XInputGetState(0, &m_CurrentState);
	if (InputState == ERROR_NOT_CONNECTED)
		std::cout << "No controller found.\n";
	else if (InputState != ERROR_SUCCESS)
		std::cout << "Something went wrong with the input.\n";

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {

		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {

		}
	}

	for (ControllerAction& action : m_actions)
	{
		if (IsPressed(action.button))
		{
			if (CheckPressingState(action)) {
				action.command->Execute(m_pPlayerOne);
			}

			action.isDown = true;
		}
		else
		{
			if (action.state == PressingState::buttonUp && action.isDown)
				action.command->Execute(m_pPlayerOne);

			action.isDown = false;
		}

		if (action.state == PressingState::ThumbStick)
		{
			float RX = m_CurrentState.Gamepad.sThumbRX;
			float RY = m_CurrentState.Gamepad.sThumbRY;
			float magnitude = sqrt(RX * RX + RY * RY);

			if (magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE * 3)
			{
				//clip the magnitude at its expected maximum value
				if (magnitude > 32767) magnitude = 32767;

				//adjust magnitude relative to the end of the dead zone
				magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE * 3;

				action.command->Execute(m_pPlayerOne, glm::vec2{ RX, RY });
			}
		}
	}

	return true;
}

bool kaas::InputManager::IsPressed(ControllerButton button) const
{
	switch (button)
	{
	case kaas::ControllerButton::ButtonA:
		if (m_CurrentState.Gamepad.wButtons == XINPUT_GAMEPAD_A)
			return true;
		break;
	case kaas::ControllerButton::ButtonB:
		if (m_CurrentState.Gamepad.wButtons == XINPUT_GAMEPAD_B)
			return true;
		break;
	case kaas::ControllerButton::ButtonX:
		if (m_CurrentState.Gamepad.wButtons == XINPUT_GAMEPAD_X)
			return true;
		break;
	case kaas::ControllerButton::ButtonY:
		if (m_CurrentState.Gamepad.wButtons == XINPUT_GAMEPAD_Y)
			return true;
	case kaas::ControllerButton::DPAD_LEFT:
		if (m_CurrentState.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_LEFT)
			return true;
	case kaas::ControllerButton::DPAD_RIGHT:
		if (m_CurrentState.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_RIGHT)
			return true;
	case kaas::ControllerButton::DPAD_UP:
		if (m_CurrentState.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_UP)
			return true;
	case kaas::ControllerButton::DPAD_DOWN:
		if (m_CurrentState.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_DOWN)
			return true;
		break;
	default: return false;
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
	case kaas::PressingState::ThumbStick:

		break;
	}
	return false;
}

void kaas::InputManager::SetPlayerOne(GameObject* pPlayerObject)
{
	m_pPlayerOne = pPlayerObject;
}