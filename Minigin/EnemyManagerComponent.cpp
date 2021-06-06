#include "MiniginPCH.h"
#include "EnemyManagerComponent.h"
#include <fstream>
#include <string>
#include "../3rdParty/rapidjson/rapidjson.h"
#include "../3rdParty/rapidjson/reader.h"
#include "../3rdParty/rapidjson/document.h"
#include "Timer.h"
#include "LevelComponent.h"
#include "SlickSamControllerComponent.h"
#include "CharacterControllerComponent.h"
#include "CoilyComponent.h"
#include "UggWrongWayComponent.h"
#include "TransformComponent.h"


using namespace rapidjson;
kaas::EnemyManagerComponent::EnemyManagerComponent(GameObject* pGameObject, std::string enemyListFilePath, CharacterControllerComponent* pPlayerController)
	:BaseComponent{pGameObject}
	,m_PastTime{0.0f}
	,m_CurrentLevel{0}
	,m_LastCheckedEnemy{0}
	,m_pPlayerController{pPlayerController}
{
	m_pLevel = pGameObject->GetComponent<LevelComponent>();

	if (!m_pLevel)
		std::cout << "level not found in EnemyManager\n";

	Document doc;

	std::ifstream file(enemyListFilePath);
	std::stringstream buffer;
	buffer << file.rdbuf();

	doc.Parse(std::string(buffer.str()).c_str());
	const Value& flyingDiscs = doc["Enemies"];

	//Check if the data we got has an array
	if (flyingDiscs.IsArray())
		for (SizeType i = 0; i < flyingDiscs.Size(); i++)
		{
			EnemieData enemyData{};
			bool isLeft{};
			for (Value::ConstMemberIterator itr = flyingDiscs[i].MemberBegin(); itr != flyingDiscs[i].MemberEnd(); ++itr)
			{
				if (itr->name == "EnemyType")
					enemyData.type = enemieTypes(itr->value.GetInt());
				if (itr->name == "SpawnTime")
					enemyData.spawnTime = float(itr->value.GetInt());
				if (itr->name == "Level")
					enemyData.level = itr->value.GetInt();
			}

			m_Enemies.push_back(enemyData);
		}
}

kaas::EnemyManagerComponent::~EnemyManagerComponent()
{
	for (GameObject* pGameObject : m_EnemyObjects)
	{
		delete pGameObject;
		pGameObject = nullptr;
	}
}

void kaas::EnemyManagerComponent::Update()
{
	m_PastTime += Timer::GetInstance().GetDeltaTime();

	if (m_LastCheckedEnemy != m_Enemies.size() - 1) 
	{
		if (m_Enemies[m_LastCheckedEnemy].spawnTime <= m_PastTime && m_Enemies[m_LastCheckedEnemy].level == m_CurrentLevel)
		{
			switch (m_Enemies[m_LastCheckedEnemy].type)
			{
				case enemieTypes::Coily:
				{
					GameObject* pGameObject = new GameObject{};
					TransformComponent* pTransformComp = new TransformComponent{ pGameObject, glm::vec2{0.0f, 0.0f} };
					CharacterControllerComponent* pCharacterComponent = new CharacterControllerComponent{ pGameObject, m_pLevel, TileAffection::onlyActive, true, true };
					CoilyComponent* pSlickSlamComp = new CoilyComponent{ pGameObject, pCharacterComponent, m_pPlayerController };
					m_EnemyObjects.push_back(pGameObject);
				}
				break;
				case enemieTypes::SlickSam:
				{
					GameObject* pGameObject = new GameObject{};
					TransformComponent* pTransformComp = new TransformComponent{ pGameObject, glm::vec2{0.0f, 0.0f} };
					CharacterControllerComponent* pCharacterComponent = new CharacterControllerComponent{ pGameObject, m_pLevel, TileAffection::onlyActive, true, true };
					SlickSamControllerComponent* pSlickSlamComp = new SlickSamControllerComponent{ pGameObject, pCharacterComponent };
					m_EnemyObjects.push_back(pGameObject);
					break;
				}
				case enemieTypes::UggWrongway:
				{
					GameObject* pGameObject = new GameObject{};
					TransformComponent* pTransformComp = new TransformComponent{ pGameObject, glm::vec2{0.0f, 0.0f} };
					CharacterControllerComponent* pCharacterComponent = new CharacterControllerComponent{ pGameObject, m_pLevel, TileAffection::nothing, true, true };
					UggWrongWayComponent* pUggComp = new UggWrongWayComponent{ pGameObject, pCharacterComponent };
					m_EnemyObjects.push_back(pGameObject);
					break;
				}
			}
		}
	}

	for (GameObject* pGameObject : m_EnemyObjects)
	{
		if (pGameObject->GetActive())
			pGameObject->Update();
	}
}

void kaas::EnemyManagerComponent::Render() const
{
	for (GameObject* pGameObject : m_EnemyObjects)
	{
		if (pGameObject->GetActive())
			pGameObject->Render();
	}
}

void kaas::EnemyManagerComponent::NextLevel()
{
	m_PastTime = 0.0f;
	m_CurrentLevel++;

	//Remove all enemies that didn't spawn from the past level
	m_EnemyObjects.clear();

	for (size_t i = 0; i < m_Enemies.size(); i++)
	{
		if (m_Enemies[i].level == m_CurrentLevel)
		{
			m_LastCheckedEnemy = i;
			return;
		}
	}
}

void kaas::EnemyManagerComponent::ResetEnemies()
{
	m_PastTime = 0.0f;
	m_CurrentLevel = 0;
	m_LastCheckedEnemy = 0;
}
