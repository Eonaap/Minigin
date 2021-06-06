#pragma once
#include "BaseComponent.h"
#include "TransformComponent.h"
#include "LevelComponent.h"

namespace kaas
{

	class CharacterControllerComponent : public BaseComponent
	{
	public:
		CharacterControllerComponent(GameObject* pGameObject, LevelComponent* pLevel, TileAffection affectsTiles, bool diesByFall = false, bool canJumpOff = true);

		void Update() override;
		
		void SetTarget(int direction);

		void SetTargetByID(int tileID);

		void KillCharacter();
		void LoseLife();
		void FireEvent(Event event);
		int GetCurrentRow() const;
		int GetCurrentID() const;

		void PrepareOtherModes(GameObject* pPlayerTwo, bool modeIsCoop);
	private:
		int m_CurrentTileID, m_CurrentTargetID, m_CurrentRow;
		glm::vec2 m_TargetPos;
		TransformComponent* m_pTransform;
		LevelComponent* m_pLevel;
		TileAffection m_TileAffection;
		bool m_IsMoving, m_JumpedInVoid, m_TookDamage, m_IsOnDisc, m_DiesByFall, m_CanJumpOff;
		float m_Offset, m_MovementSpeed;
	};
}