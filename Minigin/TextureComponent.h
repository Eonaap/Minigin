#pragma once
#include "BaseComponent.h"
#include "Texture2D.h"
#include <SDL.h>
namespace kaas 
{
	class TransformComponent;
	class TextureComponent final: public BaseComponent
	{
	public:
		TextureComponent(GameObject* pGameObject, std::string fileLocation);
		~TextureComponent() override;
		void Update() override;
		void Render() const override;
		void SetTexture(std::string fileLocation);

		void SetTextureSize(int width, int height);
	private:
		Texture2D* m_pTexture;
		SDL_Rect m_Dst;
		TransformComponent* m_pTransform;
	};
}


