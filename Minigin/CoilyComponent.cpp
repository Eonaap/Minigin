#include "QBertGamePCH.h"
#include "CoilyComponent.h"
#include "CharacterControllerComponent.h"
#include "EnemyManagerComponent.h"
#include "TextureComponent.h"
#include <stdlib.h>
#include <time.h>

CoilyComponent::CoilyComponent(kaas::GameObject* pGameObject, CharacterControllerComponent* pControllerComponent, CharacterControllerComponent* pPlayerController, EnemyManagerComponent* pEnemyManager, bool isAIController)
	:BaseComponent{pGameObject}
	,m_pController{pControllerComponent}
	,m_pPlayerController{pPlayerController}
	,m_pEnemyManager{pEnemyManager}
	,m_IsDown{false}
	,m_IsAIController{isAIController}
{
	kaas::TextureComponent* pTexture = new kaas::TextureComponent{ pGameObject, "../Data/CoilyBall.png" };

	/* initialize random seed: */
	srand(unsigned int(time(NULL)));
}

void CoilyComponent::Update()
{
	if (m_IsDown)
	{	
		if (m_pPlayerController->GetCurrentID() == m_pController->GetCurrentID()) 
		{
			m_pPlayerController->LoseLife();

			if (m_IsAIController)
				m_pEnemyManager->ResetEnemies();
		}

		if (!m_IsAIController)
			return;

		int playerRow = m_pPlayerController->GetCurrentRow();
		int coilyRow = m_pController->GetCurrentRow();

		if (playerRow == coilyRow)
		{
			CalculatePlayerOnSameRow(coilyRow);
		}
		else if (playerRow > coilyRow)
		{
			CalculatePlayerOnHigherRow(playerRow);
		}
		else
		{
			CalculatePlayerOnLowerRow(playerRow);
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
			m_pGameObject->GetComponent<kaas::TextureComponent>()->SetTexture("../Data/Coily.png");
		}
	}
}

void CoilyComponent::CalculatePlayerOnSameRow(int coilyRow)
{
	if (m_pPlayerController->GetCurrentID() < m_pController->GetCurrentID())
	{
		//If coily is not on the last row, jump down
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
		//If coily is not on the last row, jump down
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

void CoilyComponent::CalculatePlayerOnHigherRow(int playerRow)
{
	int bottomLeftID = m_pPlayerController->GetCurrentID() - playerRow;

	if (bottomLeftID == m_pController->GetCurrentID()) 
	{
		m_pController->SetTarget(int(MovementDirections::bottomLeft));
	}
	else if (bottomLeftID + 1 == m_pController->GetCurrentID())
	{
		m_pController->SetTarget(int(MovementDirections::bottomRight));
	}
	else
	{
		int random = rand() % 2;
		m_pController->SetTarget(2 + random);
	}
}

void CoilyComponent::CalculatePlayerOnLowerRow(int playerRow)
{
	int topLeftID = m_pPlayerController->GetCurrentID() + playerRow + 1;

	if (topLeftID == m_pController->GetCurrentID()) 
	{
		m_pController->SetTarget(int(MovementDirections::topLeft));
	}
	else if (topLeftID - 1 == m_pController->GetCurrentID()) 
	{
		m_pController->SetTarget(int(MovementDirections::topRight));
	}
	else
	{
		int random = rand() % 2;
		m_pController->SetTarget(random);
	}
}