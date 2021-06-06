#pragma once
#include "BaseComponent.h"

namespace kaas 
{
	class CharacterControllerComponent;

	class SlickSamControllerComponent : public BaseComponent
	{
	public:
		SlickSamControllerComponent(GameObject* pGameObject, CharacterControllerComponent* pControllerComponent, CharacterControllerComponent* pPlayerController);

		void Update() override;
	private:
		CharacterControllerComponent* m_pController;
		CharacterControllerComponent* m_pPlayerController;
	};
}