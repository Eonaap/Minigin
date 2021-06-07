#pragma once
#include "BaseComponent.h"

class EnemyManagerComponent;
class CharacterControllerComponent;
class CoilyComponent : public kaas::BaseComponent
{
public:
	CoilyComponent(kaas::GameObject* pGameObject, CharacterControllerComponent* pControllerComponent, CharacterControllerComponent* pPlayerController, EnemyManagerComponent* pEnemyManager, bool isAIController = true);

	void Update() override;
private:

	void CalculatePlayerOnSameRow(int coilyRow);
	void CalculatePlayerOnHigherRow(int playerRow);
	void CalculatePlayerOnLowerRow(int playerRow);
	CharacterControllerComponent* m_pController;
	CharacterControllerComponent* m_pPlayerController;
	EnemyManagerComponent* m_pEnemyManager;
	bool m_IsDown, m_IsAIController;
};
	
