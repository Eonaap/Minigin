#include "QBertGamePCH.h"
#include "MainMenuComponent.h"
#include "ResourceManager.h"
#include "Components.h"
#include "Structs.h"
#include "Renderer.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "AudioLocator.h"
#include "AudioManager.h"

MainMenuComponent::MainMenuComponent(kaas::GameObject* pGameObject, kaas::GameObject* pPlayerOne, kaas::GameObject* pPlayerTwo)
	:BaseComponent{pGameObject}
	,m_ActiveButton{2}
	,m_pPlayerOne{pPlayerOne}
	,m_pPlayerTwo{pPlayerTwo}
	,m_ControllerEnterButton{}
	,m_ControllerNextButton{}
{
	m_pSingleButtonActive = kaas::ResourceManager::GetInstance().LoadTexture("../Data/SingleButton.png");
	m_pCoopButtonActive = kaas::ResourceManager::GetInstance().LoadTexture("../Data/CoopButton.png");
	m_pVersusButtonActive = kaas::ResourceManager::GetInstance().LoadTexture("../Data/VersusButton.png");

	m_ControllerEnterButton.button = ControllerButton::ButtonA;
	m_ControllerEnterButton.state = PressingState::buttonDown;

	m_ControllerNextButton.button = ControllerButton::DPAD_RIGHT;
	m_ControllerNextButton.state = PressingState::buttonDown;
}

MainMenuComponent::~MainMenuComponent()
{
	delete m_pSingleButtonActive;
	m_pSingleButtonActive = nullptr;

	delete m_pVersusButtonActive;
	m_pVersusButtonActive = nullptr;

	delete m_pCoopButtonActive;
	m_pCoopButtonActive = nullptr;
}

void MainMenuComponent::Update()
{

	if (kaas::InputManager::GetInstance().KeyIsPressed(SDLK_d) || kaas::InputManager::GetInstance().ProcessControllerButton(m_ControllerNextButton, 1))
		{
			m_ControllerNextButton.isDown = true;
			m_ActiveButton++;
			if (m_ActiveButton > 2)
				m_ActiveButton = 0;

			kaas::AudioManager* manager = static_cast<kaas::AudioManager*>(kaas::AudioLocator::getAudio());
			manager->AddRequest(0, 19, 0);
		}

	if (kaas::InputManager::GetInstance().KeyIsPressed(SDLK_RETURN) || kaas::InputManager::GetInstance().ProcessControllerButton(m_ControllerEnterButton, 1))
	{
		switch (m_ActiveButton)
		{
		case 1:
			m_pPlayerTwo->SetActive(true);
			m_pPlayerOne->GetComponent<CharacterControllerComponent>()->PrepareOtherModes(m_pPlayerTwo, true);
			break;
		case 2:
			m_pPlayerTwo->SetActive(true);
			m_pPlayerOne->GetComponent<CharacterControllerComponent>()->PrepareOtherModes(m_pPlayerTwo, false);
			break;
		}

		kaas::AudioManager* manager = static_cast<kaas::AudioManager*>(kaas::AudioLocator::getAudio());
		manager->AddRequest(1, 19, 0);
		kaas::SceneManager::GetInstance().SetActiveScene("Game");
	}
}

void MainMenuComponent::Render() const
{
	float x, y, w, h;
	//Render Single button

	switch (m_ActiveButton)
	{
	case 0:
		x = 40.0f;
		y = 175.0f;
		w = 255.0f;
		h = 85.0f;
		kaas::Renderer::GetInstance().RenderTexture(*m_pSingleButtonActive, x, y, w, h);
		break;
	case 1:
		x = 350.0f;
		y = 170.0f;
		w = 275.0f;
		h = 90.0f;
		kaas::Renderer::GetInstance().RenderTexture(*m_pCoopButtonActive, x, y, w, h);
		break;
	case 2:
		x = 165.0f;
		y = 318.0f;
		w = 277.0f;
		h = 75.0f;
		kaas::Renderer::GetInstance().RenderTexture(*m_pVersusButtonActive, x, y, w, h);
		break;
	}
	
}
