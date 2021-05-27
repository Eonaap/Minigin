#pragma once
#include "Singleton.h"

namespace kaas 
{
	class Time : public Singleton<Time>
	{
	public:
		float GetDeltaTime() const;
		void SetDeltaTime(float dT);
	private:
		float m_DeltaTime;
	};
}
