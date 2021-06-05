#pragma once
#include "BaseComponent.h"

enum class enemieTypes 
{
	SlickSam,
	UggWrongway,
	Coily
};

struct EnemieData
{
	enemieTypes type;
	float spawnTime;
	int level;
};

namespace kaas 
{
	class LevelComponent;

	class EnemyManagerComponent : public BaseComponent
	{
	public:
		EnemyManagerComponent(GameObject* pGameObject, std::string enemyListFilePath);
		~EnemyManagerComponent();
		void Update() override;
		void Render() const override;
		void NextLevel();
	private:
		LevelComponent* m_pLevel;
		std::vector<EnemieData> m_Enemies;
		std::vector<GameObject*> m_EnemyObjects;
		float m_PastTime;
		int m_CurrentLevel;
	};
}