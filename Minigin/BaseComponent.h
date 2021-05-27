#pragma once
#include "GameObject.h"

namespace kaas
{
	class BaseComponent
	{
	public:
		BaseComponent(GameObject* pGameObject);
		virtual ~BaseComponent() = default;
		virtual void Update() = 0;
		virtual void Render() const;
	protected:
		GameObject* m_pGameObject;
	};
}



