#pragma once
#include "BaseComponent.h"
#include "HealthComponent.h"
namespace kaas
{
	class CharacterComponent : public BaseComponent
	{
	public:
		CharacterComponent(GameObject* gameObject, HealthComponent* healthComponent);
		void Update() override;
	private:
		HealthComponent* m_pHealthComp;
	};
}

