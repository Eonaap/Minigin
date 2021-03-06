#include "MiniginPCH.h"
#include "TextComponent.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

kaas::TextComponent::TextComponent(GameObject* pGameObject, std::string text, Font* pFont)
	:BaseComponent{pGameObject}
	,m_Text{text}
	,m_pFont{pFont}
	,m_Pos{}
	,m_NeedsUpdate{true}
{
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	if (transform)
		m_Pos = transform->GetPosition();
	
}

kaas::TextComponent::~TextComponent()
{
	delete m_pTexture;
	m_pTexture = nullptr;

	delete m_pFont;
	m_pFont = nullptr;
}

void kaas::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		const SDL_Color color = { 255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);

		delete m_pTexture;
		m_pTexture = new Texture2D{ texture };
		m_NeedsUpdate = false;
	}
}

void kaas::TextComponent::Render() const
{
	if (m_pTexture != nullptr) 
	{
		Renderer::GetInstance().RenderTexture(*m_pTexture, m_Pos.x, m_Pos.y);
	}
}

void kaas::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void kaas::TextComponent::SetPosOffset(glm::vec2 pos)
{
	m_Pos += pos;
}
