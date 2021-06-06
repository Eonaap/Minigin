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
	SetupInput();
	SetupScene();
}

void QBert::SetupInput() const
{
}

void QBert::SetupScene() const
{

	GameObject* pBackground = new GameObject();
	//Create end menu scene
	{
		Scene& Endscene = SceneManager::GetInstance().CreateScene("EndMenu");

		//Background
		TextureComponent* pBackgroundTexture = new TextureComponent{ pBackground, "../Data/EndMenu.png" };
		pBackgroundTexture->SetTextureSize(640, 480);
		EndMenuComponent* pEndMenuComponent = new EndMenuComponent{ pBackground };

		Endscene.Add(pBackground);
	}

	glm::vec2 pos{};
	auto& scene = SceneManager::GetInstance().CreateScene("Game");

	auto go = new GameObject{};
	TextureComponent* textureComp = new TextureComponent{ go, "background.jpg" };
	scene.Add(go);

	//Create the level object
	//Create the levelComponent outside of the backets, to send it to th characterComp
	auto QBertObject = new GameObject{};
	auto QBertObject2 = new GameObject{};
	auto pFont3 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto pFont4 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto pFont5 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 50);
	pos.x = 25.0f;
	pos.y = 100.0f;
	TransformComponent* transComp4 = new TransformComponent{ QBertObject, pos };
	TransformComponent* transComp5 = new TransformComponent{ QBertObject2, pos };
	TextComponent* textComp2 = new TextComponent{ QBertObject, " ", pFont3 };
	TextComponent* textComp3 = new TextComponent{ QBertObject, "", pFont4 };
	TextComponent* textComp4 = new TextComponent{ pBackground, "", pFont5 };
	StatsObserver* pStats = new StatsObserver{ 3, textComp2, textComp3, textComp4 };
	StatsObserver* pStats2 = new StatsObserver{ 3, textComp2, textComp3, textComp4 };
	StatsObserver* pStats3 = new StatsObserver{ 3, textComp2, textComp3, textComp4 };
	auto LevelObject = new GameObject{};
	LevelObject->GetSubject()->AddObserver(pStats);

	auto pFont2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	pos.x = 25.0f;
	pos.y = 50.0f;
	TransformComponent* transComp3 = new TransformComponent{ LevelObject, pos };
	TextComponent* pLevelTextComponent = new TextComponent{ LevelObject, "Level 1", pFont2 };
	LevelComponent* pLevelComponent = new LevelComponent{ LevelObject, "../Data/LevelDataScaled.json" , "../Data/Tiles.png", "../Data/Disc.png", glm::vec2{50, 55}, glm::vec2{275, 70} };
	scene.Add(LevelObject);
	
	//Create the player
	{
		CharacterControllerComponent* pCharacterController = new CharacterControllerComponent{ QBertObject, pLevelComponent, TileAffection::always };
		TextureComponent* pCharacterTextureComp = new TextureComponent{ QBertObject, "../Data/QBert.png" };
		QBertObject->GetSubject()->AddObserver(pStats2);
		InputComponent* pInputComponent = new InputComponent{ QBertObject, 1 };

		pInputComponent->AddAction(ControllerAction{ ControllerButton::RightThumbStick, SDLK_BACKQUOTE, new MoveCommand{}, PressingState::ThumbStick, false });
		pInputComponent->AddAction(ControllerAction{ ControllerButton::RightThumbStick, SDLK_s, new MoveBottomRightCommand{}, PressingState::buttonDown, false });
		pInputComponent->AddAction(ControllerAction{ ControllerButton::RightThumbStick, SDLK_a, new MoveBottomLeftCommand{}, PressingState::buttonDown, false });
		pInputComponent->AddAction(ControllerAction{ ControllerButton::RightThumbStick, SDLK_q, new MoveTopLeftCommand{}, PressingState::buttonDown, false });
		pInputComponent->AddAction(ControllerAction{ ControllerButton::RightThumbStick, SDLK_w, new MoveTopRightCommand{}, PressingState::buttonDown, false });
		scene.Add(QBertObject);

		//Create the EnemyManagercomponent with the characterController
		EnemyManagerComponent* pEnemyManagerComponent = new EnemyManagerComponent{ LevelObject, "../Data/EnemiesData.json", pCharacterController };
	}

	//Create a second player
	{

		CharacterControllerComponent* pCharacterController = new CharacterControllerComponent{ QBertObject2, pLevelComponent, TileAffection::always };
		QBertObject->GetSubject()->AddObserver(pStats3);
		InputComponent* pInputComponent = new InputComponent{ QBertObject2, 2 };

		pInputComponent->AddAction(ControllerAction{ ControllerButton::LeftThumbStick, SDLK_BACKQUOTE, new MoveCommand{}, PressingState::ThumbStick, false });
		pInputComponent->AddAction(ControllerAction{ ControllerButton::LeftThumbStick, SDLK_g, new MoveBottomRightCommand{}, PressingState::buttonDown, false });
		pInputComponent->AddAction(ControllerAction{ ControllerButton::LeftThumbStick, SDLK_f, new MoveBottomLeftCommand{}, PressingState::buttonDown, false });
		pInputComponent->AddAction(ControllerAction{ ControllerButton::LeftThumbStick, SDLK_r, new MoveTopLeftCommand{}, PressingState::buttonDown, false });
		pInputComponent->AddAction(ControllerAction{ ControllerButton::LeftThumbStick, SDLK_t, new MoveTopRightCommand{}, PressingState::buttonDown, false });
		scene.Add(QBertObject2);
	}

	//Create the audioManager in the audioLocator and add 2 sounds
	AudioLocator::provide(new AudioManager());
	AudioLocator::getAudio()->AddSound("../Data/Music.wav");
	AudioLocator::getAudio()->AddSound("../Data/DerpNugget.wav");

	//Create Main menu scene
	{
		Scene& Startscene = SceneManager::GetInstance().CreateScene("MainMenu");

		//Background
		GameObject* pBackgroundMainMenu = new GameObject();
		TextureComponent* pBackgroundTexture = new TextureComponent{ pBackgroundMainMenu, "../Data/MainMenu.png" };
		pBackgroundTexture->SetTextureSize(640, 480);
		MainMenuComponent* pMainMenuComponent = new MainMenuComponent{ pBackgroundMainMenu, QBertObject, QBertObject2 };

		Startscene.Add(pBackgroundMainMenu);
	}

	SceneManager::GetInstance().SetActiveScene("MainMenu");
}
