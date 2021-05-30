#include "MiniginPCH.h"
#include "Subject.h"

kaas::Subject::Subject()
{
	m_pObservers = std::vector<Observer*>();
}
