#include "MiniginPCH.h"
#include "MainMenuComponent.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "Structs.h"
#include "Renderer.h"
#include "InputManager.h"
#include "SceneManager.h"

kaas::MainMenuComponent::MainMenuComponent(GameObject* pGameObject)
	:BaseComponent{pGameObject}
	,m_ActiveButton{2}
{
	m_pSingleButtonActive = ResourceManager::GetInstance().LoadTexture("../Data/SingleButton.png");
	m_pCoopButtonActive = ResourceManager::GetInstance().LoadTexture("../Data/CoopButton.png");
	m_pVersusButtonActive = ResourceManager::GetInstance().LoadTexture("../Data/VersusButton.png");
}

kaas::MainMenuComponent::~MainMenuComponent()
{
	delete m_pSingleButtonActive;
	m_pSingleButtonActive = nullptr;

	delete m_pVersusButtonActive;
	m_pVersusButtonActive = nullptr;

	delete m_pCoopButtonActive;
	m_pCoopButtonActive = nullptr;
}

void kaas::MainMenuComponent::Update()
{
	if (InputManager::GetInstance().KeyIsPressed(SDLK_d))
		{
			m_ActiveButton++;
			if (m_ActiveButton > 2)
				m_ActiveButton = 0;
		}

	if (InputManager::GetInstance().KeyIsPressed(SDLK_RETURN))
	{
		switch (m_ActiveButton)
		{
		case 0:
			//Open single mode
			SceneManager::GetInstance().SetActiveScene("SingleMode");
			break;
		case 1:
			//Open coop mode
			SceneManager::GetInstance().SetActiveScene("CoopMode");
			break;
		case 2:
			//Open versus
			SceneManager::GetInstance().SetActiveScene("VersusMode");
			break;
		}
	}
}

void kaas::MainMenuComponent::Render() const
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
