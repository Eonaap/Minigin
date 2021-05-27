#include "MiniginPCH.h"
#include "Time.h"

float kaas::Time::GetDeltaTime() const
{
    return m_DeltaTime;
}

void kaas::Time::SetDeltaTime(float dT)
{
    m_DeltaTime = dT;
}
