#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "Time.h"
#include "TextComponent.h"

kaas::FPSComponent::FPSComponent(GameObject* pGameObject)
	:BaseComponent{pGameObject}
{
}

void kaas::FPSComponent::Update()
{
	m_ElapsedTime += Time::GetInstance().GetDeltaTime();

	if (m_ElapsedTime > 1.0f) {
		TextComponent* textComp = m_pGameObject->GetComponent<TextComponent>();
		if (textComp)
		{
			textComp->SetText(std::to_string(m_FPS));
		}
		m_FPS = 0;
		m_ElapsedTime = 0.0f;
	}
	else
	{
		m_FPS++;
	}

}