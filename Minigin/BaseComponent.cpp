#include "MiniginPCH.h"
#include "BaseComponent.h"

kaas::BaseComponent::BaseComponent(GameObject* pGameObject)
	:m_pGameObject{pGameObject}
{
}

void kaas::BaseComponent::Render() const
{
}
