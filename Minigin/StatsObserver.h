#pragma once
#include "BaseComponent.h"
#include "Subject.h"
#include "TextComponent.h"
#include "SceneManager.h"

namespace kaas
{
	class StatsObserver : public Observer
	{
	public:
		StatsObserver(int maxHealth, TextComponent* pHealthText, TextComponent* pPointsText, TextComponent* pPointsTextEndMenu)
			:m_MaxHealth{maxHealth}
			,m_CurrentHealth{ maxHealth }
			,m_Points{0}
		{
			m_pHealthText = pHealthText;
			m_pHealthText->SetText(std::to_string(maxHealth));

			m_pPointsText = pPointsText;
			m_pPointsText->SetText(std::to_string(m_Points));
			m_pPointsText->SetPosOffset(glm::vec2{ 0.0f, 75.0f });

			m_pPointsTextEndMenu = pPointsTextEndMenu;
			m_pPointsTextEndMenu->SetText(std::to_string(m_Points));
			m_pPointsTextEndMenu->SetPosOffset(glm::vec2{ 50.0f, 50.0f });
		};

		~StatsObserver() 
		{
		}

		void onNotify(const GameObject& gameObject, const Event& eventInfo) override
		{
			UNREFERENCED_PARAMETER(gameObject);

			switch (eventInfo)
			{
			case Event::LoseLife:
				if (m_IsAlive) 
				{
					m_CurrentHealth--;
					if (m_CurrentHealth > 0)
					{
						m_pHealthText->SetText(std::to_string(m_CurrentHealth));
					}
					else
					{
						m_IsAlive = false;
						SceneManager::GetInstance().SetActiveScene("EndMenu");
					}
				}
				break;
			case Event::ColorChange:
				m_Points += 25;
				m_pPointsText->SetText(std::to_string(m_Points));
				m_pPointsTextEndMenu->SetText(std::to_string(m_Points));
				break;
			case Event::Coilydefeat:
				m_Points += 500;
				m_pPointsText->SetText(std::to_string(m_Points));
				m_pPointsTextEndMenu->SetText(std::to_string(m_Points));
				break;
			case Event::DiscRemained:
				m_Points += 50;
				m_pPointsText->SetText(std::to_string(m_Points));
				m_pPointsTextEndMenu->SetText(std::to_string(m_Points));
				break;
			case Event::CatchSlickSam:
				m_Points += 300;
				m_pPointsText->SetText(std::to_string(m_Points));
				m_pPointsTextEndMenu->SetText(std::to_string(m_Points));
				break;
			}
		};

	private:
		int m_MaxHealth, m_CurrentHealth, m_Points;
		bool m_IsAlive = true;
		TextComponent* m_pHealthText;
		TextComponent* m_pPointsText;
		TextComponent* m_pPointsTextEndMenu;
	};
}