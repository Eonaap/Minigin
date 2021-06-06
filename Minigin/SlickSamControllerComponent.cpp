#include "MiniginPCH.h"
#include "SlickSamControllerComponent.h"
#include "CharacterControllerComponent.h"
#include "TextureComponent.h"
#include <stdlib.h>
#include <time.h>

kaas::SlickSamControllerComponent::SlickSamControllerComponent(GameObject* pGameObject, CharacterControllerComponent* pControllerComponent, CharacterControllerComponent* pPlayerController)
	:BaseComponent{pGameObject}
	,m_pController{ pControllerComponent }
	,m_pPlayerController{pPlayerController}
{
	TextureComponent* pTexture = new TextureComponent{ pGameObject, "../Data/Slick.png" };

	/* initialize random seed: */
	srand(unsigned int(time(NULL)));
}

void kaas::SlickSamControllerComponent::Update()
{
	int random = rand() % 2;

	m_pController->SetTarget(2 + random);

	if (m_pController->GetCurrentID() == m_pPlayerController->GetCurrentID())
	{
		m_pPlayerController->FireEvent(Event::CatchSlickSam);
		m_pGameObject->SetActive(false);
	}
}
