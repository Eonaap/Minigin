#pragma once
#include "BaseComponent.h"
#pragma warning (disable:4201)
#pragma warning (disable:4189)
#include <glm.hpp>

namespace kaas 
{
	class TransformComponent final : public BaseComponent
	{
	public:
		TransformComponent(GameObject* pGameObject, glm::vec2 pos);
		void Update() override;
		void Render() const override;
		glm::vec2 GetPosition() const;
		void SetPosition(glm::vec2& pos);
		void Move(glm::vec2& pos);
	private:
		glm::vec2 m_Position;
	};
}