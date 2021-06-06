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
		std::cout << "No controller found.\n";
	else if (InputState != ERROR_SUCCESS)
		std::cout << "Something went wrong with the input.\n";

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYUP) {
			m_Keys[e.key.keysym.sym] = false;
			m_KeysPreviousUpdate[e.key.keysym.sym] = false;
		}
		if (e.type == SDL_KEYDOWN) {
			m_Keys[e.key.keysym.sym] = true;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {

		}
	}

	return true;
}

bool kaas::InputManager::ProcessControllerButton(ControllerAction& button)
{
	if (IsPressed(button.button))
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
	UNREFERENCED_PARAMETER(pPlayerObject);
	//m_pPlayerOne = pPlayerObject;
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
