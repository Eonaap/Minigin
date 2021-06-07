#include "MiniginPCH.h"
#include "InputManager.h"
#include "Command.h"

kaas::InputManager::InputManager()
{
	for (int i = 0; i < 322; i++) { // init them all to false
		m_Keys[i] = false;
		m_KeysPreviousUpdate[i] = false;
	}
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
		std::cout << "first controller not found.\n";
	else if (InputState != ERROR_SUCCESS)
		std::cout << "Something went wrong with the input for the first controller.\n";

	ZeroMemory(&m_CurrentStateP2, sizeof(XINPUT_STATE));
	DWORD InputState2 = XInputGetState(1, &m_CurrentStateP2);
	if (InputState2 == ERROR_NOT_CONNECTED)
		std::cout << "Second controller not found.\n";
	else if (InputState2 != ERROR_SUCCESS)
		std::cout << "Something went wrong with the input for the second controller.\n";

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYUP) {
			if (e.key.keysym.sym < m_KeySize) 
			{
				m_Keys[e.key.keysym.sym] = false;
				m_KeysPreviousUpdate[e.key.keysym.sym] = false;
			}	
		}
		if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym < m_KeySize)
				m_Keys[e.key.keysym.sym] = true;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {

		}
	}

	return true;
}

bool kaas::InputManager::ProcessControllerButton(ControllerAction& button, int playerID)
{
	if (IsPressed(button.button, playerID))
	{
		if (CheckPressingState(button)) {
			return true;
		}

		button.isDown = true;
	}
	else
	{
		if (button.state == PressingState::buttonUp && button.isDown)
			return true;

		button.isDown = false;
	}

	return false;
}

bool kaas::InputManager::IsPressed(ControllerButton button, int playerID) const
{
	XINPUT_STATE state = playerID == 1 ? m_CurrentState : m_CurrentStateP2;

	switch (button)
	{
	case ControllerButton::ButtonA:
		if (state.Gamepad.wButtons == XINPUT_GAMEPAD_A)
			return true;
		break;
	case ControllerButton::ButtonB:
		if (state.Gamepad.wButtons == XINPUT_GAMEPAD_B)
			return true;
		break;
	case ControllerButton::ButtonX:
		if (state.Gamepad.wButtons == XINPUT_GAMEPAD_X)
			return true;
		break;
	case ControllerButton::ButtonY:
		if (state.Gamepad.wButtons == XINPUT_GAMEPAD_Y)
			return true;
	case ControllerButton::DPAD_LEFT:
		if (state.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_LEFT)
			return true;
	case ControllerButton::DPAD_RIGHT:
		if (state.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_RIGHT)
			return true;
	case ControllerButton::DPAD_UP:
		if (state.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_UP)
			return true;
	case ControllerButton::DPAD_DOWN:
		if (state.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_DOWN)
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
	case PressingState::buttonPressed:
		return true;
		break;
	case PressingState::buttonDown:
		if (!button.isDown)
		{
			return true;
		}
		break;
	case PressingState::ThumbStick:

		break;
	}
	return false;
}

bool kaas::InputManager::KeyIsPressed(SDL_KeyCode scanCode)
{
	if (m_Keys[scanCode] && m_KeysPreviousUpdate[scanCode] == false)
	{
		m_KeysPreviousUpdate[scanCode] = true;
		return true;
	}
	else
	{
		return false;
	}
}

XINPUT_STATE kaas::InputManager::GetCurrentState() const
{
	return m_CurrentState;
}

XINPUT_STATE kaas::InputManager::GetCurrentStateP2() const
{
	return m_CurrentStateP2;
}
