#pragma once
#include "BaseComponent.h"

class CharacterControllerComponent;

class SlickSamControllerComponent : public kaas::BaseComponent
{
public:
	SlickSamControllerComponent(kaas::GameObject* pGameObject, CharacterControllerComponent* pControllerComponent, CharacterControllerComponent* pPlayerController);

	void Update() override;
private:
	CharacterControllerComponent* m_pController;
	CharacterControllerComponent* m_pPlayerController;
};