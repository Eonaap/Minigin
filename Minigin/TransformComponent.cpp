#include "MiniginPCH.h"
#include "TransformComponent.h"

kaas::TransformComponent::TransformComponent(GameObject* pGameObject, glm::vec2 pos)
	:BaseComponent{pGameObject}
	,m_Position{pos}
{
}

void kaas::TransformComponent::Update()
{
}

void kaas::TransformComponent::Render() const
{
}

glm::vec2 kaas::TransformComponent::GetPosition() const
{
	return m_Position;
}

void kaas::TransformComponent::SetPosition(glm::vec2& pos)
{
	m_Position = pos;
}
