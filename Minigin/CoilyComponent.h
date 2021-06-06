#pragma once
#include "BaseComponent.h"

namespace kaas 
{
	class EnemyManagerComponent;
	class CharacterControllerComponent;
	class CoilyComponent : public BaseComponent
	{
	public:
		CoilyComponent(GameObject* pGameObject, CharacterControllerComponent* pControllerComponent, CharacterControllerComponent* pPlayerController, EnemyManagerComponent* pEnemyManager);

		void Update() override;
	private:
		CharacterControllerComponent* m_pController;
		CharacterControllerComponent* m_pPlayerController;
		EnemyManagerComponent* m_pEnemyManager;
		bool m_GoesToLeftSide, m_IsDown;
	};
}