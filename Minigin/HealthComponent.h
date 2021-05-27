#pragma once
#include "BaseComponent.h"
namespace kaas 
{
	class HealthComponent : public BaseComponent
	{
	public:
		HealthComponent(GameObject* gameObject, float maxHealth)
			:BaseComponent{gameObject}
			,m_MaxHealth{maxHealth}
			,m_CurrentHealth{ maxHealth }{};

		void AddHealth(float amount)
		{
			if (m_CurrentHealth + amount <= m_MaxHealth)
			{
				m_CurrentHealth += amount;
				m_IsAlive = true;
			}
			else if (m_CurrentHealth + amount <= 0.0f)
			{
				m_CurrentHealth = 0.0f;
				m_IsAlive = false;
			}
			else
			{
				m_CurrentHealth += m_MaxHealth;
			}
		};

		const bool IsAlive() { return m_IsAlive; };
		const float& GetHealth() { return m_CurrentHealth; };
	private:
		float m_MaxHealth, m_CurrentHealth;
		bool m_IsAlive = true;
	};
}