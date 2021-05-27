#pragma once
#include "BaseComponent.h"

namespace kaas 
{
	class Font;
	class Texture2D;
	class TextComponent final: public BaseComponent
	{
	public:
		TextComponent(GameObject* pGameObject, std::string text, Font* pFont);
		~TextComponent();
		void Update() override;
		void Render() const;

		void SetText(const std::string& text);
	private:
		std::string m_Text;
		Font* m_pFont;
		Texture2D* m_pTexture;
		bool m_NeedsUpdate;
	};
}


