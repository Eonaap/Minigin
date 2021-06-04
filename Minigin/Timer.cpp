#include "MiniginPCH.h"
#include "Timer.h"

float kaas::Timer::GetDeltaTime() const
{
    return m_DeltaTime;
}

void kaas::Timer::SetDeltaTime(float dT)
{
    m_DeltaTime = dT;
}
