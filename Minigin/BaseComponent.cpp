#include "MiniginPCH.h"
#include "BaseComponent.h"

kaas::BaseComponent::BaseComponent(GameObject* pGameObject)
	:m_pGameObject{pGameObject}
{
	pGameObject->AddComponent(this);
}

void kaas::BaseComponent::Render() const
{
}