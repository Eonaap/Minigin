#pragma once
#include "BaseComponent.h"

namespace kaas 
{
	class EnemyManagerComponent;
	class CharacterControllerComponent;

	class UggWrongWayComponent : public BaseComponent
	{
	public:
		UggWrongWayComponent(GameObject* pGameObject, CharacterControllerComponent* pControllerComponent, CharacterControllerComponent* pPlayerController, EnemyManagerComponent* pEnemyManager);

		void Update() override;
	private:
		CharacterControllerComponent* m_pController;
		CharacterControllerComponent* m_pPlayerController;
		EnemyManagerComponent* m_pEnemyManager;
		bool m_GoesToLeftSide;
	};
}