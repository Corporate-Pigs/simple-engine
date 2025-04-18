#pragma once

#include <vector>

#include "SE.h"

namespace SimpleEngine
{


struct Counter
{

   public:
    struct State
    {
        const float m_value;
        const double m_time;
    };
    Counter(const std::vector<State> p_states);
    ~Counter();
    float GetFloatValueFromCurrentState();
    int32_t GetIntValueFromCurrentState();

    
   public:
    const enum class Type { UNDEFINED = 0, DELTA, STATE } m_type;
    double m_timeSinceChange;

    /*
    const T m_initialValue;
    const T m_finalValue;
    const T m_deltaValue;
    const double m_time;*/
    uint32_t m_currentStateIndex;
    const std::vector<State> m_states;

    void Update(const double p_elaspedTimeInSeconds);
};

}  // namespace SimpleEngine
