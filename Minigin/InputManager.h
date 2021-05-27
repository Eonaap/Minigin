#pragma once
#include <Windows.h>
#include <XInput.h>
#pragma comment(lib,"XInput.lib")
#include <vector>
#include "Command.h"
namespace kaas
{
	enum class ControllerButton
	{
		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY,
		//todo: add the other buttons
	};

	enum class PressingState
	{
		buttonPressed,
		buttonUp,
		buttonDown
	};

	struct ControllerAction
	{
		ControllerButton button{};
		Command* command{};
		PressingState state{};
		bool isDown{};
	};

	class InputManager
	{
	public:
		InputManager();
		~InputManager();
		void ProcessInput();
		bool CheckPressingState(ControllerAction& button);
		bool IsPressed(ControllerButton button) const;
	private:
		XINPUT_STATE m_pCurrentState{};
		std::vector<ControllerAction> m_actions;
	};
}