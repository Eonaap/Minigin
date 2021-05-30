#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include "Command.h"

kaas::InputManager::InputManager()
{
	m_actions.push_back(ControllerAction{ ControllerButton::ButtonA, new LifeCommand{}, PressingState::buttonUp, false });
	m_actions.push_back(ControllerAction{ ControllerButton::ButtonB, new DuckCommand{}, PressingState::buttonDown, false });
	m_actions.push_back(ControllerAction{ ControllerButton::ButtonX, new FireCommand{}, PressingState::buttonUp, false });
	m_actions.push_back(ControllerAction{ ControllerButton::ButtonY, new ShieldCommand{}, PressingState::buttonPressed, false });
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
	}
	return false;
}

void kaas::InputManager::SetPlayerOne(GameObject* pPlayerObject)
{
	m_pPlayerOne = pPlayerObject;
}