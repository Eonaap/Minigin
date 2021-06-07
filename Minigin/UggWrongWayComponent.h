#pragma once
#include "BaseComponent.h"

class EnemyManagerComponent;
class CharacterControllerComponent;

class UggWrongWayComponent : public kaas::BaseComponent
{
public:
	UggWrongWayComponent(kaas::GameObject* pGameObject, CharacterControllerComponent* pControllerComponent, CharacterControllerComponent* pPlayerController, EnemyManagerComponent* pEnemyManager);

	void Update() override;
private:
	CharacterControllerComponent* m_pController;
	CharacterControllerComponent* m_pPlayerController;
	EnemyManagerComponent* m_pEnemyManager;
	bool m_GoesToLeftSide;
};