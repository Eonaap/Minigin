#pragma once
#include "BaseComponent.h"
#include "Structs.h"
namespace kaas 
{
	class Texture2D;
}

class EndMenuComponent : public kaas::BaseComponent
{
public:
	EndMenuComponent(kaas::GameObject* pGameObject);
	~EndMenuComponent();
	void Update() override;
	void Render() const override;
private:
	kaas::Texture2D* m_pContinueButtonActive;
	kaas::Texture2D* m_pQuitButtonActive;
	kaas::Texture2D* m_pWonBackground;
	kaas::Texture2D* m_pLostBackground;
	int m_ActiveButton;
	ControllerAction m_ControllerEnterButton;
	ControllerAction m_ControllerNextButton;
};
