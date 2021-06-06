#include "MiniginPCH.h"
#include "UggWrongWayComponent.h"
#include "CharacterControllerComponent.h"
#include "EnemyManagerComponent.h"
#include "TextureComponent.h"
#include <stdlib.h>
#include <time.h>

kaas::UggWrongWayComponent::UggWrongWayComponent(GameObject* pGameObject, CharacterControllerComponent* pControllerComponent, CharacterControllerComponent* pPlayerController, EnemyManagerComponent* pEnemyManager)
	:BaseComponent{ pGameObject }
	, m_pController{ pControllerComponent }
	,m_pPlayerController{pPlayerController}
	,m_pEnemyManager{pEnemyManager}
{
	TextureComponent* pTexture = new TextureComponent{ pGameObject, "../Data/Ugg.png" };

	/* initialize random seed: */
	srand(unsigned int(time(NULL)));

	int random = rand() % 2;
	if (random == 0)
	{
		m_pController->SetTargetByID(21);
		m_GoesToLeftSide = false;
	}
	else
	{
		m_pController->SetTargetByID(27);
		m_GoesToLeftSide = true;
	}
}

void kaas::UggWrongWayComponent::Update()
{
	if (m_pController->GetCurrentID() == m_pPlayerController->GetCurrentID())
	{
		m_pPlayerController->LoseLife();
		m_pEnemyManager->ResetEnemies();
	}

	int random = rand() % 2;

	if (m_GoesToLeftSide)
	{
		if (random == 0)
		{
			m_pController->SetTarget(int(MovementDirections::topLeft));
		}
		else
		{
			m_pController->SetTarget(int(MovementDirections::UggWrongWayLeft));
		}
	}
	else
	{
		if (random == 0)
		{
			m_pController->SetTarget(int(MovementDirections::topRight));
		}
		else
		{
			m_pController->SetTarget(int(MovementDirections::UggWrongWayRight));
		}
	}
}
