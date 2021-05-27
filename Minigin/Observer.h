#pragma once
#include "GameObject.h"

struct Event {

};

namespace kaas
{
	class Observer
	{
	public:
		virtual ~Observer() {};
		virtual void onNotify(const GameObject& gameObject, const Event& eventInfo) = 0;
	};
}