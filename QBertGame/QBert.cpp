#include "QBertGamePCH.h"
#include "QBert.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "HealthComponent.h"
#include "AudioLocator.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "EnemyManagerComponent.h"
#include "Components.h"
#pragma warning (disable:4201)
#pragma warning (disable:4189)
#include <glm.hpp>

using namespace kaas;

void QBert::LoadGame() const
{
	SetupInput();
	SetupScene();
}

void QBert::SetupInput() const
{
}

void QBert::SetupScene() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto go = new GameObject{};
	TextureComponent* textureComp = new TextureComponent{ go, "background.jpg" };
	scene.Add(go);

	auto go2 = new GameObject{};
	auto pFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	TextComponent* textComp = new TextComponent{ go2, "Programming 4 Assignment", pFont };
	glm::vec2 pos{};
	pos.x = 80.0f;
	pos.y = 20.0f;
	TransformComponent* transComp1 = new TransformComponent{ go2, pos };
	scene.Add(go2);

	//Create the level object
	auto LevelObject = new GameObject{};
	auto pFont2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	pos.x = 25.0f;
	pos.y = 50.0f;
	TransformComponent* transComp3 = new TransformComponent{ LevelObject, pos };
	TextComponent* pLevelTextComponent = new TextComponent{ LevelObject, "Level 1", pFont2 };
	LevelComponent* pLevelComponent = new LevelComponent{ LevelObject, "../Data/LevelDataScaled.json" , "../Data/Tiles.png", "../Data/Disc.png", glm::vec2{50, 55}, glm::vec2{275, 70} };
	EnemyManagerComponent* pEnemyManagerComponent = new EnemyManagerComponent{LevelObject, "../Data/EnemiesData.json" };
	scene.Add(LevelObject);

	//Create the player
	auto QBertObject = new GameObject{};
	auto pFont3 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	pos.x = 25.0f;
	pos.y = 100.0f;
	TransformComponent* transComp4 = new TransformComponent{ QBertObject, pos };
	TextComponent* textComp2 = new TextComponent{ QBertObject, " ", pFont3 };

	CharacterControllerComponent* pCharacterController = new CharacterControllerComponent{ QBertObject, pLevelComponent, TileAffection::always};
	TextureComponent* pCharacterTextureComp = new TextureComponent{ QBertObject, "../Data/QBert.png" };
	QBertObject->GetSubject()->AddObserver(new HealthComponent{ 3, QBertObject });

	scene.Add(QBertObject);

	//Put QBert as player one in the inputManager
	InputManager::GetInstance().SetPlayerOne(QBertObject);

	//Create the audioManager in the audioLocator and add 2 sounds
	AudioLocator::provide(new AudioManager());
	AudioLocator::getAudio()->AddSound("../Data/Music.wav");
	AudioLocator::getAudio()->AddSound("../Data/DerpNugget.wav");
}
