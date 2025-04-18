#pragma once

#include "SE_counter.h"

namespace SimpleEngine
{

struct Counter;

struct CounterService
{
    friend struct Counter;
    friend class Game;

   private:
    static CounterService& Instance()
    {
        static CounterService s_instance;
        return s_instance;
    }

    void RegisterForUpdates(Counter *p_counterVariantPtr);
    void UnregisterForUpdates(Counter *p_counterVariantPtr);
    void UpdateCounters(const double p_elapsedTimeInSeconds);

    // Delete copy/move to enforce single instance
    CounterService(const CounterService&) = delete;
    CounterService& operator=(const CounterService&) = delete;
    CounterService(CounterService&&) = delete;
    CounterService& operator=(CounterService&&) = delete;

    CounterService() = default;
    std::vector<Counter*> m_counters;
    int32_t GetCounterIndexIfExists(const Counter* p_counterVariantPtr);
};

}  // namespace SimpleEngine