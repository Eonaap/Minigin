#include "MiniginPCH.h"
#include "CoilyComponent.h"
#include "CharacterControllerComponent.h"
#include "TextureComponent.h"
#include <stdlib.h>
#include <time.h>

kaas::CoilyComponent::CoilyComponent(GameObject* pGameObject, CharacterControllerComponent* pControllerComponent, CharacterControllerComponent* pPlayerController)
	:BaseComponent{pGameObject}
	,m_pController{pControllerComponent}
	,m_pPlayerController{pPlayerController}
	,m_IsDown{false}
{
	TextureComponent* pTexture = new TextureComponent{ pGameObject, "../Data/Coily.png" };

	/* initialize random seed: */
	srand(unsigned int(time(NULL)));
}

void kaas::CoilyComponent::Update()
{
	if (m_IsDown)
	{
		if (m_pPlayerController->GetCurrentID() == m_pController->GetCurrentID())
			m_pPlayerController->KillCharacter();

		int playerRow = m_pPlayerController->GetCurrentRow();
		int coilyRow = m_pController->GetCurrentRow();

		if (playerRow == coilyRow)
		{
			if (m_pPlayerController->GetCurrentID() < m_pController->GetCurrentID())
			{
				if (coilyRow != 7)
				{
					m_pController->SetTarget(int(MovementDirections::bottomLeft));
				}
				else
				{
					m_pController->SetTarget(int(MovementDirections::topLeft));
				}
			}
			else
			{
				if (coilyRow != 7)
				{
					m_pController->SetTarget(int(MovementDirections::bottomRight));
				}
				else
				{
					m_pController->SetTarget(int(MovementDirections::topRight));
				}
			}
				
		}

		if (playerRow > coilyRow)
		{
			if (m_pPlayerController->GetCurrentID() - playerRow == m_pController->GetCurrentID())
				m_pController->SetTarget(int(MovementDirections::bottomLeft));
			else
			if (m_pPlayerController->GetCurrentID() - playerRow + 1 == m_pController->GetCurrentID())
				m_pController->SetTarget(int(MovementDirections::bottomRight));
			else 
			{
				int random = rand() % 2;

				m_pController->SetTarget(2 + random);
			}
		}
		else
		{
			if (m_pPlayerController->GetCurrentID() + playerRow + 1 == m_pController->GetCurrentID())
				m_pController->SetTarget(int(MovementDirections::topLeft));
			else
				if (m_pPlayerController->GetCurrentID() + playerRow == m_pController->GetCurrentID())
					m_pController->SetTarget(int(MovementDirections::topRight));
				else
				{
					int random = rand() % 2;

					m_pController->SetTarget(random);
				}
		}
	}
	else
	{
		int random = rand() % 2;

		m_pController->SetTarget(2 + random);

		//If coily is at the latest row, start the second behaviour
		if (m_pController->GetCurrentRow() == 7)
		{
			m_IsDown = true;
			m_pGameObject->GetComponent<TextureComponent>()->SetTexture("../Data/Coily2.png");
		}
	}
}
