#include "MiniginPCH.h"
#include "EndMenuComponent.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "Structs.h"
#include "Renderer.h"
#include "InputManager.h"
#include "SceneManager.h"

kaas::EndMenuComponent::EndMenuComponent(GameObject* pGameObject)
	:BaseComponent{ pGameObject }
	,m_ActiveButton{ 0 }
{
	m_pContinueButtonActive = ResourceManager::GetInstance().LoadTexture("../Data/SingleButton.png");
	m_pQuitButtonActive = ResourceManager::GetInstance().LoadTexture("../Data/CoopButton.png");
	m_pWonBackground = ResourceManager::GetInstance().LoadTexture("../Data/EndMenyWon.png");
	m_pLostBackground = ResourceManager::GetInstance().LoadTexture("../Data/EndMenu.png");
}

kaas::EndMenuComponent::~EndMenuComponent()
{
	delete m_pContinueButtonActive;
	m_pContinueButtonActive = nullptr;
	
	delete m_pQuitButtonActive;
	m_pQuitButtonActive = nullptr;
	
	delete m_pWonBackground;
	m_pWonBackground = nullptr;
	
	delete m_pLostBackground;
	m_pLostBackground = nullptr;
}

void kaas::EndMenuComponent::Update()
{
	if (InputManager::GetInstance().KeyIsPressed(SDLK_d))
	{
		m_ActiveButton++;
		if (m_ActiveButton > 1)
			m_ActiveButton = 0;
	}

	if (InputManager::GetInstance().KeyIsPressed(SDLK_RETURN))
	{
		switch (m_ActiveButton)
		{
		case 0:
			//Open Main menu
			SceneManager::GetInstance().SetActiveScene("MainMenu");
			break;
		case 1:
			//Quit the application
			SceneManager::GetInstance().SetActiveScene("CoopMode");
			break;
		}
	}
}

void kaas::EndMenuComponent::Render() const
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
		kaas::Renderer::GetInstance().RenderTexture(*m_pContinueButtonActive, x, y, w, h);
		break;
	case 1:
		x = 350.0f;
		y = 170.0f;
		w = 275.0f;
		h = 90.0f;
		kaas::Renderer::GetInstance().RenderTexture(*m_pQuitButtonActive, x, y, w, h);
		break;
	}

}

