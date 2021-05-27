#pragma once
#include "BaseComponent.h"
#include "Texture2D.h"

namespace kaas 
{
	class TextureComponent final: public BaseComponent
	{
	public:
		TextureComponent(GameObject* pGameObject, std::string fileLocation);
		~TextureComponent() override;
		void Update() override;
		void Render() const override;
	private:
		Texture2D* m_pTexture;
	};
}


