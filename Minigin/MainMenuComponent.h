#pragma once
#include "BaseComponent.h"
#include "Structs.h"
namespace kaas 
{
	class Texture2D;
	class MainMenuComponent : public BaseComponent
	{
	public:
		MainMenuComponent(GameObject* pGameObject, GameObject* pPlayerOne, GameObject* pPlayerTwo);
		~MainMenuComponent();
		void Update() override;
		void Render() const override;
	private:
		Texture2D* m_pSingleButtonActive;
		Texture2D* m_pVersusButtonActive;
		Texture2D* m_pCoopButtonActive;
		int m_ActiveButton;
		GameObject* m_pPlayerOne;
		GameObject* m_pPlayerTwo;
		
		ControllerAction m_ControllerEnterButton;
		ControllerAction m_ControllerNextButton;

	};
}