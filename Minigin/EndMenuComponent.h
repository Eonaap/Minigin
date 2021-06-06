#pragma once
#include "BaseComponent.h"
#include "Structs.h"
namespace kaas
{
	class Texture2D;
	class EndMenuComponent : public BaseComponent
	{
	public:
		EndMenuComponent(GameObject* pGameObject);
		~EndMenuComponent();
		void Update() override;
		void Render() const override;
	private:
		Texture2D* m_pContinueButtonActive;
		Texture2D* m_pQuitButtonActive;
		Texture2D* m_pWonBackground;
		Texture2D* m_pLostBackground;
		int m_ActiveButton;
		ControllerAction m_ControllerEnterButton;
		ControllerAction m_ControllerNextButton;
	};
}