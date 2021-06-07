#pragma once
#include "Singleton.h"

namespace kaas 
{
	class Timer : public Singleton<Timer>
	{
	public:
		float GetDeltaTime() const;
		void SetDeltaTime(float dT);
	private:
		float m_DeltaTime{};
	};
}
