#pragma once
#include "BaseComponent.h"
#include "Structs.h"

namespace kaas 
{
	class Texture2D;
}

class MainMenuComponent : public kaas::BaseComponent
{
public:
	MainMenuComponent(kaas::GameObject* pGameObject, kaas::GameObject* pPlayerOne, kaas::GameObject* pPlayerTwo);
	~MainMenuComponent();
	void Update() override;
	void Render() const override;
private:
	kaas::Texture2D* m_pSingleButtonActive;
	kaas::Texture2D* m_pVersusButtonActive;
	kaas::Texture2D* m_pCoopButtonActive;
	int m_ActiveButton;
	kaas::GameObject* m_pPlayerOne;
	kaas::GameObject* m_pPlayerTwo;

	ControllerAction m_ControllerEnterButton;
	ControllerAction m_ControllerNextButton;

};
