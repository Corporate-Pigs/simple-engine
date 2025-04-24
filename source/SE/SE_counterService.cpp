#include "SE_counterService.h"

#include <cstdint>
#include <cassert>

void SimpleEngine::CounterService::RegisterForUpdates(Counter *p_counterVariantPtr)
{
    if (GetCounterIndexIfExists(p_counterVariantPtr) == -1)
    {
        m_counters.push_back(p_counterVariantPtr);
    }
}

void SimpleEngine::CounterService::UnregisterForUpdates(Counter *p_counterVariantPtr)
{
    int32_t index = GetCounterIndexIfExists(p_counterVariantPtr);
    assert(index > -1);
    m_counters.erase(m_counters.begin() + index);
}

void SimpleEngine::CounterService::UpdateCounters(const double p_elapsedTimeInSeconds)
{
    for (Counter *counterPtr : m_counters)
    {
        counterPtr->Update(p_elapsedTimeInSeconds);
    }
}

int32_t SimpleEngine::CounterService::GetCounterIndexIfExists(const Counter *p_counterVariantPtr)
{
    for (uint32_t i = 0; i < m_counters.size(); i++)
    {
        if (m_counters[i] == p_counterVariantPtr)
        {
            return i;
        }
    }
    return -1;
}
