#pragma once
#include <XInput.h>
#include "Singleton.h"
namespace kaas
{
	class GameObject;
	class Command;

	enum class ControllerButton
	{
		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY,
		DPAD_LEFT,
		DPAD_RIGHT,
		DPAD_UP,
		DPAD_DOWN,
		RightThumbStick
	};

	enum class PressingState
	{
		buttonPressed,
		buttonUp,
		buttonDown,
		ThumbStick
	};

	struct ControllerAction
	{
		ControllerButton button{};
		Command* command{};
		PressingState state{};
		bool isDown{};
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();
		bool ProcessInput();
		bool IsPressed(ControllerButton button) const;
		bool CheckPressingState(ControllerAction& button);
		void SetPlayerOne(GameObject* pPlayerObject);
	private:
		XINPUT_STATE m_CurrentState{};
		std::vector<ControllerAction> m_actions;
		GameObject* m_pPlayerOne;
	};

}
