#pragma once
#include "BaseComponent.h"

namespace kaas 
{
	class Texture2D;
	class MainMenuComponent : public BaseComponent
	{
	public:
		MainMenuComponent(GameObject* pGameObject);

		void Update() override;
		void Render() const override;
	private:
		Texture2D* m_pSingleButtonActive;
		Texture2D* m_pVersusButtonActive;
		Texture2D* m_pCoopButtonActive;
		int m_ActiveButton;
	};
}