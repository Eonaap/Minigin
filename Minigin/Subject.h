#pragma once
#include <vector>
#include "Observer.h"

namespace kaas 
{
	class Subject
	{
	public:

	protected:
		void notify(const GameObject& gameObject, const Event& eventInfo)
		{
			for (auto observer : m_pObservers)
			{
				observer->onNotify(gameObject, eventInfo);
			}
		}
	private:
		std::vector<Observer*> m_pObservers;
	};
}