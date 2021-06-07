#include "QBertGamePCH.h"
#include "Subject.h"
#include "Timer.h"
#include <glm.hpp>
#include <math.h>
#include "TransformComponent.h"
#include "LevelComponent.h"
#include "TextureComponent.h"
#include "CoilyComponent.h"
#include "CharacterControllerComponent.h"
#include "AudioManager.h"
#include "AudioLocator.h"

CharacterControllerComponent::CharacterControllerComponent(kaas::GameObject* pGameObject, LevelComponent* pLevel, TileAffection affectsTiles, bool diesByFall, bool canJumpOff)
	:BaseComponent{ pGameObject }
	,m_CurrentTileID{0}
	,m_CurrentTargetID{-1}
	,m_CurrentRow{1}
	,m_TargetPos{ }
	,m_pLevel{pLevel}
	,m_IsMoving{false}
	,m_JumpedInVoid{false}
	,m_CanJumpOff{ canJumpOff }
	,m_TileAffection{affectsTiles}
	,m_DiesByFall{diesByFall}
	,m_Offset{15.0f}
	,m_MovementSpeed{4.0f}
{
	m_pTransform = pGameObject->GetComponent<kaas::TransformComponent>();

	if (!m_pTransform)
		std::cout << "Failed to find TransformComponent on gameObject.\n";
	else
	{
		glm::vec2 pos = pLevel->GetTilePos(0);
		m_pTransform->SetPosition(pos);
	}
}

void CharacterControllerComponent::Update()
{
	if (m_IsMoving)
	{
		glm::vec2 movement{};

		movement = (m_pTransform->GetPosition() + (m_TargetPos - m_pTransform->GetPosition()) * kaas::Timer::GetInstance().GetDeltaTime() * m_MovementSpeed);

		if (glm::length(m_TargetPos - movement) < 3.0f)
		{
			m_pTransform->SetPosition(m_TargetPos);

			if (!m_JumpedInVoid)
			{
				m_IsMoving = false;
				m_CurrentTileID = m_CurrentTargetID;

				if (m_TileAffection != TileAffection::nothing)
				{
					m_pLevel->SetTileState(m_CurrentTileID, m_TileAffection);
				}
					

				if (m_pLevel->GetLevelFinished())
				{
					m_TargetPos = m_pLevel->GetTilePos(0);
					m_CurrentTargetID = 0;
					m_CurrentRow = 1;
					m_IsMoving = true;
				}
			}
			else
			{
				if (m_DiesByFall)
				{
					KillCharacter();
					return;
				}


				if (!m_IsOnDisc)
				{
					if (m_pLevel->IsOnDisc(m_pTransform->GetPosition()))
					{
						m_TargetPos = m_pLevel->GetDiscEndLocation();
						m_CurrentTargetID = 0;
						m_CurrentRow = 1;
						m_IsMoving = true;
						m_IsOnDisc = true;

						kaas::AudioManager* manager = static_cast<kaas::AudioManager*>(kaas::AudioLocator::getAudio());
						manager->AddRequest(2, 19, 0);
					}
					else
					{
						//Add call to observer for death
						LoseLife();
						m_TookDamage = true;
						m_TargetPos = m_pLevel->GetTilePos(0);
						m_CurrentTargetID = 0;
						m_CurrentRow = 1;
						m_IsMoving = true;
						m_JumpedInVoid = false;
					}
				}
				else
				{
					m_IsOnDisc = false;
					m_TargetPos = m_pLevel->GetTilePos(0);
					m_CurrentTargetID = 0;
					m_CurrentRow = 1;
					m_IsMoving = true;
					m_JumpedInVoid = false;
				}
				
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

void CharacterControllerComponent::SetTarget(int direction)
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
		case int(MovementDirections::UggWrongWayLeft) :
			newTileID = m_CurrentTileID - 1;
			break;

		case int(MovementDirections::UggWrongWayRight) :
			newTileID = m_CurrentTileID  + 1;
			break;
		}

		//to prevent the player to go from one side of the pyramid to the other
		//if going from the left side to top left dir, the previous tile needs to be on the same row
		//if going from the right side to top right dir, the next tile needs to be on the same row
		//Ref for calculating row in numerical pyramid https://stackoverflow.com/questions/37513699/find-row-of-pyramid-based-on-index
		switch (direction)
		{
			case int(MovementDirections::topLeft) :
			{
				if (int(ceil((-1 + sqrt(1 + 8 * (m_CurrentTileID + 1))) / 2)) != int(ceil((-1 + sqrt(1 + 8 * (m_CurrentTileID))) / 2)))
				{
					if (m_CanJumpOff)
					{
						m_TargetPos = m_pLevel->GetVoidPos(m_CurrentTileID - m_CurrentRow + 1, true);
						m_IsMoving = true;
						m_JumpedInVoid = true;
					}

					return;
				}
				break;
			}
			
			case int(MovementDirections::topRight) :
			{
				if (int(ceil((-1 + sqrt(1 + 8 * (m_CurrentTileID + 1))) / 2)) != int(ceil((-1 + sqrt(1 + 8 * (m_CurrentTileID + 2))) / 2)))
				{
					if (m_CanJumpOff)
					{
						int correctID = m_CurrentTileID - m_CurrentRow;
						if (correctID < 0)
							correctID = 0;

						m_TargetPos = m_pLevel->GetVoidPos(correctID, false);
						m_IsMoving = true;
						m_JumpedInVoid = true;
					}
					return;
				}
				break;
			}
			
			case int(MovementDirections::bottomLeft) :
			{
				if (m_CurrentRow == 7)
				{
					if (m_CanJumpOff)
					{
						int correctID = m_CurrentTileID + m_CurrentRow - 1;
						if (correctID < 0)
							correctID = 0;

						m_TargetPos = m_pLevel->GetVoidPos(correctID, false);
						m_IsMoving = true;
						m_JumpedInVoid = true;
					}
				}
				break;
			}
			
			case int(MovementDirections::bottomRight) :
			{
				if (m_CurrentRow == 7)
				{
					if (m_CanJumpOff)
					{
						int correctID = m_CurrentTileID + m_CurrentRow;
						if (correctID < 0)
							correctID = 0;

						m_TargetPos = m_pLevel->GetVoidPos(correctID, false);
						m_IsMoving = true;
						m_JumpedInVoid = true;
					}
				}
				break;
			}
			
			case int(MovementDirections::UggWrongWayLeft) :
			{
				if (int(ceil((-1 + sqrt(1 + 8 * (m_CurrentTileID + 1))) / 2)) != int(ceil((-1 + sqrt(1 + 8 * (m_CurrentTileID))) / 2)))
				{
					if (m_CanJumpOff)
					{
						m_TargetPos = m_pLevel->GetVoidPos(m_CurrentTileID, true);
						m_IsMoving = true;
						m_JumpedInVoid = true;
					}
					return;
				}
				break;
			}
			
			case int(MovementDirections::UggWrongWayRight) :
			{
				if (int(ceil((-1 + sqrt(1 + 8 * (m_CurrentTileID + 1))) / 2)) != int(ceil((-1 + sqrt(1 + 8 * (m_CurrentTileID + 2))) / 2)))
				{
					if (m_CanJumpOff)
					{
						m_TargetPos = m_pLevel->GetVoidPos(m_CurrentTileID, true);
						m_IsMoving = true;
						m_JumpedInVoid = true;
					}
					return;
				}
				break;
			}	
		}

		if (newTileID < 0 || newTileID > 27)
			return;

		if (direction != int(MovementDirections::UggWrongWayLeft) && direction != int(MovementDirections::UggWrongWayRight))
		{
			//Change the row according to the character going up or down
			if (m_CurrentTargetID < newTileID)
				m_CurrentRow++;
			else
				m_CurrentRow--;
		}

		m_CurrentTargetID = newTileID;
		m_TargetPos = m_pLevel->GetTilePos(m_CurrentTargetID);

		m_IsMoving = true;

		kaas::AudioManager* manager = static_cast<kaas::AudioManager*>(kaas::AudioLocator::getAudio());
		manager->AddRequest(0, 19, 0);
	}	
}

void CharacterControllerComponent::SetTargetByID(int tileID)
{
	m_CurrentTargetID = tileID;
	m_CurrentRow = int(ceil((-1 + sqrt(1 + 8 * (tileID + 1))) / 2));
	m_TargetPos = m_pLevel->GetTilePos(m_CurrentTargetID);

	m_IsMoving = true;
}

void CharacterControllerComponent::KillCharacter()
{
	m_pGameObject->SetActive(false);
}

void CharacterControllerComponent::LoseLife()
{
	m_pGameObject->GetSubject()->notify(*m_pGameObject, Event::LoseLife);
}

void CharacterControllerComponent::FireEvent(Event event)
{
	m_pGameObject->GetSubject()->notify(*m_pGameObject, event);
}

int CharacterControllerComponent::GetCurrentRow() const
{
	return m_CurrentRow;
}

int CharacterControllerComponent::GetCurrentID() const
{
	return m_CurrentTileID;
}

void CharacterControllerComponent::PrepareOtherModes(kaas::GameObject* pPlayerTwo, bool modeIsCoop)
{
	if (modeIsCoop)
	{
		//Player 1
		glm::vec2 pos = m_pLevel->GetTilePos(21);
		m_CurrentRow = 7;
		m_pTransform->SetPosition(pos);
		
		//Player 2
		new kaas::TextureComponent{pPlayerTwo, "../Data/QBert.png"};
		pos = m_pLevel->GetTilePos(27);
		m_CurrentRow = 7;
		pPlayerTwo->GetComponent<kaas::TransformComponent>()->SetPosition(pos);
	}
	else 
	{
		//We can make the playerController and EnemyManager a nullptr cause it's not needed for coily that's not controller by AI
		new CoilyComponent{ pPlayerTwo, pPlayerTwo->GetComponent<CharacterControllerComponent>(), this, nullptr, false };
	}
}