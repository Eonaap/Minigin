#include "QBertGamePCH.h"
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
EnemyManagerComponent::EnemyManagerComponent(kaas::GameObject* pGameObject, std::string& enemyListFilePath, CharacterControllerComponent* pPlayerController)
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
			EnemyData enemyData{};
			bool isLeft{};
			for (Value::ConstMemberIterator itr = flyingDiscs[i].MemberBegin(); itr != flyingDiscs[i].MemberEnd(); ++itr)
			{
				if (itr->name == "EnemyType")
					enemyData.type = EnemyTypes(itr->value.GetInt());
				if (itr->name == "SpawnTime")
					enemyData.spawnTime = float(itr->value.GetInt());
				if (itr->name == "Level")
					enemyData.level = itr->value.GetInt();
			}

			m_Enemies.push_back(enemyData);
		}
}

EnemyManagerComponent::~EnemyManagerComponent()
{
	for (kaas::GameObject* pGameObject : m_EnemyObjects)
	{
		delete pGameObject;
		pGameObject = nullptr;
	}
}

void EnemyManagerComponent::Update()
{
	m_PastTime += kaas::Timer::GetInstance().GetDeltaTime();

	if (m_LastCheckedEnemy != int(m_Enemies.size()) - 1) 
	{
		if (m_Enemies[m_LastCheckedEnemy].spawnTime <= m_PastTime && m_Enemies[m_LastCheckedEnemy].level == m_CurrentLevel)
		{
			switch (m_Enemies[m_LastCheckedEnemy].type)
			{
				case EnemyTypes::Coily:
				{
					kaas::GameObject* pGameObjectCoily = new kaas::GameObject{};
					kaas::TransformComponent* pTransformCompCoily = new kaas::TransformComponent{ pGameObjectCoily, glm::vec2{0.0f, 0.0f} };
					CharacterControllerComponent* pCharacterComponentCoily = new CharacterControllerComponent{ pGameObjectCoily, m_pLevel, TileAffection::nothing, true, true };
					CoilyComponent* pCoilyComp = new CoilyComponent{ pGameObjectCoily, pCharacterComponentCoily, m_pPlayerController, this };
					m_EnemyObjects.push_back(pGameObjectCoily);
					break;
				}
					
				case EnemyTypes::SlickSam:
				{
					kaas::GameObject* pGameObjectSlickSam = new kaas::GameObject{};
					kaas::TransformComponent* pTransformCompSlickSam = new kaas::TransformComponent{ pGameObjectSlickSam, glm::vec2{0.0f, 0.0f} };
					CharacterControllerComponent* pCharacterComponentSlickSam = new CharacterControllerComponent{ pGameObjectSlickSam, m_pLevel, TileAffection::onlyActive, true, true };
					SlickSamControllerComponent* pSlickSlamComp = new SlickSamControllerComponent{ pGameObjectSlickSam, pCharacterComponentSlickSam, m_pPlayerController };
					m_EnemyObjects.push_back(pGameObjectSlickSam);
					break;
				}
				
				case EnemyTypes::UggWrongway:
				{
					kaas::GameObject* pGameObjectUggWrongWay = new kaas::GameObject{};
					kaas::TransformComponent* pTransformCompUggWrongWay = new kaas::TransformComponent{ pGameObjectUggWrongWay, glm::vec2{0.0f, 0.0f} };
					CharacterControllerComponent* pCharacterComponentUggWrongWay = new CharacterControllerComponent{ pGameObjectUggWrongWay, m_pLevel, TileAffection::nothing, true, true };
					UggWrongWayComponent* pUggComp = new UggWrongWayComponent{ pGameObjectUggWrongWay, pCharacterComponentUggWrongWay, m_pPlayerController, this };
					m_EnemyObjects.push_back(pGameObjectUggWrongWay);
					break;
				}
					
			}
			m_LastCheckedEnemy++;
		}
	}

	for (kaas::GameObject* pGameObject : m_EnemyObjects)
	{
		if (pGameObject->GetActive())
			pGameObject->Update();
	}
}

void EnemyManagerComponent::Render() const
{
	for (kaas::GameObject* pGameObject : m_EnemyObjects)
	{
		if (pGameObject->GetActive())
			pGameObject->Render();
	}
}

void EnemyManagerComponent::NextLevel()
{
	m_PastTime = 0.0f;
	m_CurrentLevel++;

	//Remove all enemies that didn't spawn from the past level
	m_EnemyObjects.clear();

	for (size_t i = 0; i < m_Enemies.size(); i++)
	{
		if (m_Enemies[i].level == m_CurrentLevel)
		{
			m_LastCheckedEnemy = int(i);
			return;
		}
	}
}

void EnemyManagerComponent::ResetEnemies()
{
	m_PastTime = 0.0f;
	m_CurrentLevel = 0;
	m_LastCheckedEnemy = 0;

	//Remove all enemies that didn't spawn from the past level
	m_EnemyObjects.clear();
}
