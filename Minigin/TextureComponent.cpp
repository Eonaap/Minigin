#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"

kaas::TextureComponent::TextureComponent(GameObject* pGameObject, std::string fileLocation)
	:BaseComponent{pGameObject}
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(fileLocation);

	m_pTransform = m_pGameObject->GetComponent<TransformComponent>();

	if (m_pTransform)
	{
		m_Dst.x = int(m_pTransform->GetPosition().x);
		m_Dst.y = int(m_pTransform->GetPosition().y);
	}

	SDL_QueryTexture(m_pTexture->GetSDLTexture(), nullptr, nullptr, &m_Dst.w, &m_Dst.h);
}

kaas::TextureComponent::~TextureComponent()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void kaas::TextureComponent::Update()
{
	if (m_pTransform)
	{
		m_Dst.x = int(m_pTransform->GetPosition().x);
		m_Dst.y = int(m_pTransform->GetPosition().y);
	}
}

void kaas::TextureComponent::Render() const
{
	kaas::Renderer::GetInstance().RenderTexture(*m_pTexture, float(m_Dst.x), float(m_Dst.y), float(m_Dst.w), float(m_Dst.h));
}

void kaas::TextureComponent::SetTexture(std::string fileLocation)
{
	delete m_pTexture;
	m_pTexture = ResourceManager::GetInstance().LoadTexture(fileLocation);
}

void kaas::TextureComponent::SetTextureSize(int width, int height)
{
	m_Dst.w = width;
	m_Dst.h = height;
}
