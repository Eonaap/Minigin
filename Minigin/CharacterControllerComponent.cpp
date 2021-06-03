#include "MiniginPCH.h"
#include "CharacterControllerComponent.h"
#include "Time.h"
#include "glm/geometric.hpp"
#include <math.h>

kaas::CharacterControllerComponent::CharacterControllerComponent(GameObject* pGameObject, LevelComponent* pLevel)
	:BaseComponent{ pGameObject }
	,m_CurrentTileID{0}
	,m_CurrentTargetID{-1}
	,m_CurrentRow{1}
	,m_TargetPos{ }
	,m_pLevel{pLevel}
	,m_IsMoving{false}
	,m_Offset{15.0f}
	,m_MovementSpeed{4.0f}
{
	m_pTransform = pGameObject->GetComponent<TransformComponent>();

	if (!m_pTransform)
		std::cout << "Failed to find TransformComponent on gameObject.\n";
	else
	{
		glm::vec2 pos = pLevel->GetTile(0).pos;
		pos.x += m_Offset;
		m_pTransform->SetPosition(pos);
	}
		
}


void kaas::CharacterControllerComponent::Update()
{
	if (m_IsMoving)
	{
		glm::vec2 movement{};

		movement = (m_pTransform->GetPosition() + (m_TargetPos - m_pTransform->GetPosition()) * Time::GetInstance().GetDeltaTime() * m_MovementSpeed);

		if (glm::length(m_TargetPos - movement) < 3.0f)
		{
			m_pTransform->SetPosition(m_TargetPos);
			m_IsMoving = false;
			m_CurrentTileID = m_CurrentTargetID;
			m_pLevel->SetTileState(m_CurrentTileID);

			if (m_pLevel->GetLevelFinished())
			{
				m_TargetPos = m_pLevel->GetTile(0).pos;
				m_TargetPos.x += m_Offset;
				m_CurrentTargetID = 0;
				m_CurrentRow = 1;
				m_IsMoving = true;
			}
				
		}
		else
		{
			m_pTransform->SetPosition(movement);
		}
	}

	if (m_pTransform->GetPosition() == m_TargetPos)
	{
		m_IsMoving = false;
	}
}

void kaas::CharacterControllerComponent::SetTarget(int direction)
{
	if (!m_IsMoving)
	{
		int newTileID{ -1 };

		switch (direction)
		{
		case int(MovementDirections::topLeft) :
			newTileID = m_CurrentTileID - m_CurrentRow;
			break;
		case int(MovementDirections::topRight) :
			newTileID = m_CurrentTileID - m_CurrentRow + 1;
			break;
		case int(MovementDirections::bottomLeft) :
			newTileID = m_CurrentTileID + m_CurrentRow;
			break;
		case int(MovementDirections::bottomRight) :
			newTileID = m_CurrentTileID + m_CurrentRow + 1;
			break;
		}

		if (newTileID < 0 || newTileID > 27)
			return;

		//to prevent the player to go from one side of the pyramid to the other
		//if going from the left side to top left dir, the previous tile needs to be on the same row
		//if going from the right side to top right dir, the next tile needs to be on the same row
		//Ref for calculating row in numerical pyramid https://stackoverflow.com/questions/37513699/find-row-of-pyramid-based-on-index
		switch (direction)
		{
			case int(MovementDirections::topLeft) :
				if (int(ceil((-1 + sqrt(1 + 8 * (m_CurrentTileID + 1))) / 2)) != int(ceil((-1 + sqrt(1 + 8 * (m_CurrentTileID))) / 2)))
					return;
				break;
				case int(MovementDirections::topRight) :
					if (int(ceil((-1 + sqrt(1 + 8 * (m_CurrentTileID + 1))) / 2)) != int(ceil((-1 + sqrt(1 + 8 * (m_CurrentTileID + 2))) / 2)))
						return;
					break;
		}

		//Change the row according to the character going up or down
		if (m_CurrentTargetID < newTileID)
			m_CurrentRow++;
		else
			m_CurrentRow--;

		m_CurrentTargetID = newTileID;
		m_TargetPos = m_pLevel->GetTile(m_CurrentTargetID).pos;

		//TilePos has an offset, put it to the middle of the tile
		m_TargetPos.x += m_Offset;

		m_IsMoving = true;
	}	
}
