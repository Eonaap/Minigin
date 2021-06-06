#pragma once
#include <XInput.h>
#pragma warning (disable:26812)
#include "Singleton.h"
#include "Structs.h"

namespace kaas
{
	class GameObject;
	class Command;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();
		bool ProcessInput();
		bool ProcessControllerButton(ControllerAction& button);
		bool IsPressed(ControllerButton button) const;
		bool CheckPressingState(ControllerAction& button);
		void SetPlayerOne(GameObject* pPlayerObject);
		bool KeyIsPressed(SDL_KeyCode scanCode);

		XINPUT_STATE GetCurrentState() const;
	private:
		XINPUT_STATE m_CurrentState{};
		std::vector<ControllerAction> m_actions;

		static constexpr size_t m_KeySize{512};
		//ref to keystates https://stackoverflow.com/questions/3741055/inputs-in-sdl-on-key-pressed/3816128
		bool m_KeysPreviousUpdate[m_KeySize];
		bool m_Keys[m_KeySize];  // 322 is the number of SDLK_DOWN events
	};
}