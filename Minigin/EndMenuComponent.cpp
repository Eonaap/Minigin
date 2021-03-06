#include "QBertGamePCH.h"
#include "EndMenuComponent.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "Renderer.h"
#include "InputManager.h"
#include "SceneManager.h"

EndMenuComponent::EndMenuComponent(kaas::GameObject* pGameObject)
	:BaseComponent{ pGameObject }
	,m_ActiveButton{ 0 }
	, m_ControllerNextButton{}
	,m_ControllerEnterButton{}
{
	m_pContinueButtonActive = kaas::ResourceManager::GetInstance().LoadTexture("../Data/ContinueButton.png");
	m_pQuitButtonActive = kaas::ResourceManager::GetInstance().LoadTexture("../Data/QuitButton.png");
	m_pWonBackground = kaas::ResourceManager::GetInstance().LoadTexture("../Data/EndMenuWon.png");
	m_pLostBackground = kaas::ResourceManager::GetInstance().LoadTexture("../Data/EndMenu.png");

	m_ControllerEnterButton.button = ControllerButton::ButtonA;
	m_ControllerEnterButton.state = PressingState::buttonDown;

	m_ControllerNextButton.button = ControllerButton::DPAD_RIGHT;
	m_ControllerNextButton.state = PressingState::buttonDown;
}

EndMenuComponent::~EndMenuComponent()
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

void EndMenuComponent::Update()
{

	if (kaas::InputManager::GetInstance().KeyIsPressed(SDLK_d) || kaas::InputManager::GetInstance().ProcessControllerButton(m_ControllerNextButton, 1))
	{
		m_ControllerNextButton.isDown = true;
		m_ActiveButton++;
		if (m_ActiveButton > 1)
			m_ActiveButton = 0;
	}

	if (kaas::InputManager::GetInstance().KeyIsPressed(SDLK_RETURN) || kaas::InputManager::GetInstance().ProcessControllerButton(m_ControllerEnterButton, 1))
	{
		switch (m_ActiveButton)
		{
		case 0:
			//Open Main menu
			kaas::SceneManager::GetInstance().SetActiveScene("MainMenu");
			break;
		case 1:
			//Quit the application
			SDL_Event sdlevent;
			sdlevent.type = SDL_QUIT;

			SDL_PushEvent(&sdlevent);
			break;
		}
	}
}

void EndMenuComponent::Render() const
{
	float x{}, y{}, w{}, h{};
	//Render Single button

	switch (m_ActiveButton)
	{
	case 0:
		x = 54.0f;
		y = 180.0f;
		w = 310.0f;
		h = 60.0f;
		kaas::Renderer::GetInstance().RenderTexture(*m_pContinueButtonActive, x, y, w, h);
		break;
	case 1:
		x = 359.0f;
		y = 287.0f;
		w = 160.0f;
		h = 55.0f;
		kaas::Renderer::GetInstance().RenderTexture(*m_pQuitButtonActive, x, y, w, h);
		break;
	}

}

