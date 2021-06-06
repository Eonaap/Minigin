#pragma once
#include "Structs.h"
namespace kaas
{
	class GameObject;

	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() {};
		virtual void onNotify(const GameObject& gameObject, const Event& eventInfo) = 0;
	};
}