#include "QBertGamePCH.h"
#include "QBert.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "StatsObserver.h"
#include "AudioLocator.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "EnemyManagerComponent.h"
#include "Components.h"
#include "Command.h"
#pragma warning (disable:4201)
#pragma warning (disable:4189)
#include <glm.hpp>

using namespace kaas;

QBert::~QBert()
{
}

void QBert::LoadGame() const
{
	SetupScene();
}

void QBert::SetupInput() const
{
}

void QBert::SetupScene() const
{
	//Create all gameObjects
	GameObject* pEndMenuBackground = new GameObject();
	GameObject* pBackgroundMainMenu = new GameObject();
	GameObject* pQBertObject = new GameObject{};
	GameObject* pQBertObject2 = new GameObject{};
	GameObject* pLevelObject = new GameObject{};
	GameObject* pGameBackground = new GameObject{};

	//vec2 for putting positions
	glm::vec2 pos{0.0f, 0.0f};

	//Create the scenes
	Scene& gameScene = SceneManager::GetInstance().CreateScene("Game");
	Scene& endscene = SceneManager::GetInstance().CreateScene("EndMenu");
	Scene& Startscene = SceneManager::GetInstance().CreateScene("MainMenu");

	//Create Background of the main game
	kaas::TextureComponent* textureComp = new kaas::TextureComponent{ pGameBackground, "background.jpg" };

	//Create end menu scene
	auto pFontEndMenu = ResourceManager::GetInstance().LoadFont("Lingua.otf", 50);
	kaas::TextComponent* textCompEndMenu = new kaas::TextComponent{ pEndMenuBackground, "", pFontEndMenu };
	//Background
	kaas::TextureComponent* pBackgroundTexture = new kaas::TextureComponent{ pEndMenuBackground, "../Data/EndMenu.png" };
	pBackgroundTexture->SetTextureSize(640, 480);
	EndMenuComponent* pEndMenuComponent = new EndMenuComponent{ pEndMenuBackground };

	//Create textComponents for the health, score and to show the score in the endMenu
	auto pFontHealth = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto pFontScore = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	kaas::TextComponent* textCompHealth = new kaas::TextComponent{ pQBertObject, " ", pFontHealth };
	kaas::TextComponent* textCompScore = new kaas::TextComponent{ pQBertObject, "", pFontScore };

	//Create observers and add them to the level and 2 players
	StatsObserver* pStats = new StatsObserver{ 3, textCompHealth, textCompScore, textCompEndMenu };
	StatsObserver* pStats2 = new StatsObserver{ 3, textCompHealth, textCompScore, textCompEndMenu };
	StatsObserver* pStats3 = new StatsObserver{ 3, textCompHealth, textCompScore, textCompEndMenu };

	pLevelObject->GetSubject()->AddObserver(pStats);
	pQBertObject->GetSubject()->AddObserver(pStats2);
	pQBertObject2->GetSubject()->AddObserver(pStats3);
	
	pos.x = 25.0f;
	pos.y = 100.0f;
	//Create the transformComponents of the players and the level
	TransformComponent* pTransformPlayerOne = new TransformComponent{ pQBertObject, pos };
	TransformComponent* pTransformPlayerTwo = new TransformComponent{ pQBertObject2, pos };

	pos.x = 25.0f;
	pos.y = 50.0f;
	TransformComponent* pTransformLevel = new TransformComponent{ pLevelObject, pos };

	//Create a font for the levelNumber
	Font* pFont2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	kaas::TextComponent* pLevelTextComponent = new kaas::TextComponent{ pLevelObject, "Level 1", pFont2 };

	//Create the levelComponent
	LevelComponent* pLevelComponent = new LevelComponent{ pLevelObject, "../Data/LevelDataScaled.json" , "../Data/Tiles.png", "../Data/Disc.png", glm::vec2{50, 55}, glm::vec2{275, 70} };
	
	//Create the player
	CharacterControllerComponent* pCharacterControllerPlayerOne = new CharacterControllerComponent{ pQBertObject, pLevelComponent, TileAffection::always };
	kaas::TextureComponent* pCharacterTextureComponent = new kaas::TextureComponent{ pQBertObject, "../Data/QBert.png" };
		
	InputComponent* pInputComponentPlayerOne = new InputComponent{ pQBertObject, 1 };

	pInputComponentPlayerOne->AddAction(ControllerAction{ ControllerButton::RightThumbStick, SDLK_BACKQUOTE, new MoveCommand{}, PressingState::ThumbStick, false });
	pInputComponentPlayerOne->AddAction(ControllerAction{ ControllerButton::RightThumbStick, SDLK_s, new MoveBottomRightCommand{}, PressingState::buttonDown, false });
	pInputComponentPlayerOne->AddAction(ControllerAction{ ControllerButton::RightThumbStick, SDLK_a, new MoveBottomLeftCommand{}, PressingState::buttonDown, false });
	pInputComponentPlayerOne->AddAction(ControllerAction{ ControllerButton::RightThumbStick, SDLK_q, new MoveTopLeftCommand{}, PressingState::buttonDown, false });
	pInputComponentPlayerOne->AddAction(ControllerAction{ ControllerButton::RightThumbStick, SDLK_w, new MoveTopRightCommand{}, PressingState::buttonDown, false });

	//Create the EnemyManagercomponent with the characterController
	std::string EnemyDataFilePath{ "../Data/EnemiesData.json" };
	EnemyManagerComponent* pEnemyManagerComponent = new EnemyManagerComponent{ pLevelObject, EnemyDataFilePath, pCharacterControllerPlayerOne };

	//Create a second player
	{
		CharacterControllerComponent* pCharacterControllerPlayerTwo = new CharacterControllerComponent{ pQBertObject2, pLevelComponent, TileAffection::always };
		InputComponent* pInputComponentPlayerTwo = new InputComponent{ pQBertObject2, 1 };

		pInputComponentPlayerTwo->AddAction(ControllerAction{ ControllerButton::LeftThumbStick, SDLK_BACKQUOTE, new MoveCommand{}, PressingState::ThumbStick, false });
		pInputComponentPlayerTwo->AddAction(ControllerAction{ ControllerButton::LeftThumbStick, SDLK_g, new MoveBottomRightCommand{}, PressingState::buttonDown, false });
		pInputComponentPlayerTwo->AddAction(ControllerAction{ ControllerButton::LeftThumbStick, SDLK_f, new MoveBottomLeftCommand{}, PressingState::buttonDown, false });
		pInputComponentPlayerTwo->AddAction(ControllerAction{ ControllerButton::LeftThumbStick, SDLK_r, new MoveTopLeftCommand{}, PressingState::buttonDown, false });
		pInputComponentPlayerTwo->AddAction(ControllerAction{ ControllerButton::LeftThumbStick, SDLK_t, new MoveTopRightCommand{}, PressingState::buttonDown, false });

		//Only have the second player active if coop or versus gets enabled
		pQBertObject2->SetActive(false);
	}

	//Create Main menu scene
	kaas::TextureComponent* pBackgroundMainMenuTexture = new kaas::TextureComponent{ pBackgroundMainMenu, "../Data/MainMenu.png" };
	pBackgroundMainMenuTexture->SetTextureSize(640, 480);
	MainMenuComponent* pMainMenuComponent = new MainMenuComponent{ pBackgroundMainMenu, pQBertObject, pQBertObject2 };
	SceneManager::GetInstance().SetActiveScene("MainMenu");
	
	gameScene.Add(pGameBackground);
	gameScene.Add(pLevelObject);
	gameScene.Add(pQBertObject2);
	gameScene.Add(pQBertObject);
	endscene.Add(pEndMenuBackground);
	Startscene.Add(pBackgroundMainMenu);
	//Create the audioManager in the audioLocator and add 2 sounds
	AudioLocator::provide(new AudioManager());
	AudioLocator::getAudio()->AddSound("../Data/QBertJump.wav");
	AudioLocator::getAudio()->AddSound("../Data/LevelIntro.wav");
	AudioLocator::getAudio()->AddSound("../Data/Disk.wav");
}
