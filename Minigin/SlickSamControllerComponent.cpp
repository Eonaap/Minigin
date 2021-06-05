#include "MiniginPCH.h"
#include "SlickSamControllerComponent.h"
#include "CharacterControllerComponent.h"
#include "TextureComponent.h"

kaas::SlickSamControllerComponent::SlickSamControllerComponent(GameObject* pGameObject, CharacterControllerComponent* pControllerComponent)
	:BaseComponent{pGameObject}
	,m_pController{ pControllerComponent }
{
	m_pController->SetTargetByID(21);

	TextureComponent* pTexture = new TextureComponent{ pGameObject, "../Data/Slick.png" };
}

void kaas::SlickSamControllerComponent::Update()
{
	
}
