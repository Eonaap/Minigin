#include "QBertGamePCH.h"
#include "UggWrongWayComponent.h"
#include "CharacterControllerComponent.h"
#include "EnemyManagerComponent.h"
#include "TextureComponent.h"
#include <stdlib.h>
#include <time.h>

UggWrongWayComponent::UggWrongWayComponent(kaas::GameObject* pGameObject, CharacterControllerComponent* pControllerComponent, CharacterControllerComponent* pPlayerController, EnemyManagerComponent* pEnemyManager)
	:BaseComponent{ pGameObject }
	, m_pController{ pControllerComponent }
	,m_pPlayerController{pPlayerController}
	,m_pEnemyManager{pEnemyManager}
{
	kaas::TextureComponent* pTexture = new kaas::TextureComponent{ pGameObject, "../Data/Ugg.png" };

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

void UggWrongWayComponent::Update()
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
