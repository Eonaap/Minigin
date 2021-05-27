#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"

kaas::TextureComponent::TextureComponent(GameObject* pGameObject, std::string fileLocation)
	:BaseComponent{pGameObject}
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(fileLocation);
}

kaas::TextureComponent::~TextureComponent()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void kaas::TextureComponent::Update()
{
}

void kaas::TextureComponent::Render() const
{
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();

	glm::vec2 pos{};

	if (transform)
		pos = transform->GetPosition();

	kaas::Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}
