#pragma once
#include "BaseComponent.h"

namespace kaas
{
	class FPSComponent : public BaseComponent
	{
	public:
		FPSComponent(GameObject* pGameObject);
		void Update() override;
	private:
		int m_FPS;
		float m_ElapsedTime;
	};
}


