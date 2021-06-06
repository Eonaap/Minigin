#pragma once
#include "BaseComponent.h"

namespace kaas 
{
	class CharacterControllerComponent;

	class UggWrongWayComponent : public BaseComponent
	{
	public:
		UggWrongWayComponent(GameObject* pGameObject, CharacterControllerComponent* pControllerComponent);

		void Update() override;
	private:
		CharacterControllerComponent* m_pController;
		bool m_GoesToLeftSide;
	};
}