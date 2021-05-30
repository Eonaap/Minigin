#pragma once
#include <vector>
#include "Observer.h"
#include "GameObject.h"
namespace kaas 
{
	class Subject
	{
	public:
		Subject();

		void notify(const GameObject& gameObject, const Event& eventInfo)
		{
			for (auto observer : m_pObservers)
			{
				observer->onNotify(gameObject, eventInfo);
			}
		}

		void AddObserver(Observer* pObserver)
		{
			if (m_pObservers.empty()) {
				m_pObservers.push_back(pObserver);
			}

			bool ObserverFound{ false };
			for (Observer* pObserverInList : m_pObservers)
			{
				if (pObserver == pObserverInList) 
				{
					ObserverFound = true;
					return;
				}
			}

			if (!ObserverFound)
			{
				m_pObservers.push_back(pObserver);
			}
		}

		void RemoveObserver(Observer* pObserver)
		{
			m_pObservers.erase(std::remove(m_pObservers.begin(), m_pObservers.end(), pObserver), m_pObservers.end());
		}

	private:
		std::vector<Observer*> m_pObservers = std::vector<Observer*>();
	};
}