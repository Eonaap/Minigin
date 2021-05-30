#pragma once
#include "BaseComponent.h"
#include "Subject.h"
#include "TextComponent.h"

namespace kaas
{
	class HealthComponent : public Observer
	{
	public:
		HealthComponent(float maxHealth, GameObject* pPlayer)
			:m_MaxHealth{maxHealth}
			,m_CurrentHealth{ maxHealth }
		{
			m_pHealthText = pPlayer->GetComponent<TextComponent>();
			m_pHealthText->SetText(std::to_string(maxHealth));
		};

		void onNotify(const GameObject& gameObject, const Event& eventInfo) override
		{
			UNREFERENCED_PARAMETER(gameObject);

			switch (eventInfo)
			{
			case Event::LoseLife:
				if (m_IsAlive) 
				{
					m_CurrentHealth--;
					if (m_CurrentHealth > 0.0f)
					{
						m_pHealthText->SetText(std::to_string(m_CurrentHealth));
					}
					else
					{
						m_IsAlive = false;
						m_pHealthText->SetText("You died");
					}
				}
				break;
			default:
				break;
			}
		};

		const bool IsAlive() { return m_IsAlive; };
		const float& GetHealth() { return m_CurrentHealth; };

	private:
		float m_MaxHealth, m_CurrentHealth;
		bool m_IsAlive = true;
		TextComponent* m_pHealthText;
	};
}