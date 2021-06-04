#pragma once
#include "BaseComponent.h"
#include "TransformComponent.h"
#include "LevelComponent.h"

enum class MovementDirections
{
	topLeft,
	topRight,
	bottomLeft,
	bottomRight
};

namespace kaas
{
	class CharacterControllerComponent : public BaseComponent
	{
	public:
		CharacterControllerComponent(GameObject* pGameObject, LevelComponent* pLevel);

		void Update() override;
		
		void SetTarget(int direction);

	private:
		int m_CurrentTileID, m_CurrentTargetID, m_CurrentRow;
		glm::vec2 m_TargetPos;
		TransformComponent* m_pTransform;
		LevelComponent* m_pLevel;
		bool m_IsMoving, m_JumpedInVoid, m_TookDamage, m_IsOnDisc;
		float m_Offset, m_MovementSpeed;
	};
}