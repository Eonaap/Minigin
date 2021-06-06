#pragma once
#include "BaseComponent.h"

namespace kaas 
{
	class CharacterControllerComponent;
	class CoilyComponent : public BaseComponent
	{
	public:
		CoilyComponent(GameObject* pGameObject, CharacterControllerComponent* pControllerComponent, CharacterControllerComponent* pPlayerController);

		void Update() override;
	private:
		CharacterControllerComponent* m_pController;
		CharacterControllerComponent* m_pPlayerController;
		bool m_GoesToLeftSide, m_IsDown;
	};
}