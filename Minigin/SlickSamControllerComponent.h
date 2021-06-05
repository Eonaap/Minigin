#pragma once
#include "BaseComponent.h"

namespace kaas 
{
	class CharacterControllerComponent;

	class SlickSamControllerComponent : public BaseComponent
	{
	public:
		SlickSamControllerComponent(GameObject* pGameObject, CharacterControllerComponent* pControllerComponent);

		void Update() override;
	private:
		CharacterControllerComponent* m_pController;
	};
}