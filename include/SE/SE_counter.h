#pragma once

#include <cstdint>
#include <vector>

#include "SE_sprite.h"

namespace SimpleEngine
{

struct Counter
{
    friend struct CounterService;

   public:
    struct State
    {
        const uint32_t m_value;
        const double m_time;
    };
    Counter();
    Counter(uint32_t *p_currentValuePtr, const std::vector<State> p_states, const bool p_loop = true, const bool p_active = true);
    Counter(const std::vector<State> p_states, const bool p_loop = true, const bool p_active = true);
    Counter(const int32_t p_initialValue, const int32_t p_finalValue, const int32_t m_deltaValue, const double m_interval,
            const bool p_loop = true, const bool p_active = true);
    Counter(const int32_t p_initialValue, const int32_t p_finalValue, const double m_interval, const bool p_loop = true,
            const bool p_active = true);
    ~Counter();

    int32_t GetIntValueFromCurrentState();
    void Reset();

    bool m_isRegistered;
    bool m_done;
    bool m_active;
    bool m_loop;

   private:
    const enum class Type { UNDEFINED = 0, STATE, DELTA, TIME } m_type;
    double m_timeSinceChange;

    const uint32_t m_initialValue;
    const uint32_t m_finalValue;
    const uint32_t m_deltaValue;
    uint32_t m_currentValue;
    uint32_t *m_currentValuePtr;
    const double m_time;

    uint32_t m_currentStateIndex;
    uint32_t *m_currentStateIndexPtr;
    const std::vector<State> m_states;

    void Update(const double p_elaspedTimeInSeconds);
    void UpdateState();
    void UpdateDelta();
};

}  // namespace SimpleEngine
