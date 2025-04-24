#pragma once

#include <vector>

#include "SE.h"

namespace SimpleEngine
{

struct Counter
{
    friend struct CounterService;

   public:
    struct State
    {
        const float m_value;
        const double m_time;
    };
    Counter(const std::vector<State> p_states, const bool p_loop = true, const bool p_active = true);
    Counter(const float p_initialValue, const float p_finalValue, const float m_deltaValue, const float m_interval, const bool p_loop = true, const bool p_active = true);
    Counter(const float p_initialValue, const float p_finalValue, const float m_interval, const bool p_loop = true, const bool p_active = true);
    ~Counter();
    
    float GetFloatValueFromCurrentState();
    int32_t GetIntValueFromCurrentState();
    void Reset();

    bool m_done;
    bool m_active;
    bool m_loop;

   private:
    const enum class Type { UNDEFINED = 0, STATE, DELTA, TIME } m_type;
    double m_timeSinceChange;

    const float m_initialValue;
    const float m_finalValue;
    const float m_deltaValue;
    float m_currentValue;
    const double m_time;

    uint32_t m_currentStateIndex;
    const std::vector<State> m_states;

    void Update(const double p_elaspedTimeInSeconds);
    void UpdateState();
    void UpdateDelta();
};

}  // namespace SimpleEngine
