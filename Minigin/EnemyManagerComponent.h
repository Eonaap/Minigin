#pragma once
#include "BaseComponent.h"
#include "Structs.h"

class LevelComponent;
class CharacterControllerComponent;
class EnemyManagerComponent : public kaas::BaseComponent
{
public:
	EnemyManagerComponent(kaas::GameObject* pGameObject, std::string& enemyListFilePath, CharacterControllerComponent* pPlayerController);
	~EnemyManagerComponent();
	void Update() override;
	void Render() const override;
	void NextLevel();
	void ResetEnemies();
private:
	LevelComponent* m_pLevel;
	std::vector<EnemyData> m_Enemies;
	std::vector<kaas::GameObject*> m_EnemyObjects;
	CharacterControllerComponent* m_pPlayerController;
	float m_PastTime;
	int m_CurrentLevel, m_LastCheckedEnemy;
};