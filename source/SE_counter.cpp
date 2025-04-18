
#include "SE_counter.h"

#include "SE_counterService.h"

/*
class StateCounter : public Counter
{
    friend class Game;

   public:
    struct State
    {
        const float m_value;
        const double m_time;
    };
    Counter(const std::vector<State> p_states)
        : m_type(Type::STATE), m_timeSinceChange(0), m_currentStateIndex(0), m_states(p_states)
    {
        CounterService::Instance().RegisterForUpdates(this);
    };
    ~Counter() {}

    float GetDoubleValueFromCurrentState()
    {
        const State& currentState = m_states[m_currentStateIndex];
        return currentState.m_value;
    };

    int32_t GetIntValueFromCurrentState() { return static_cast<int32_t>(GetDoubleValueFromCurrentState()); }

   public:
    const enum class Type { UNDEFINED = 0, DELTA, STATE } m_type;
    double m_timeSinceChange;

    const T m_initialValue;
    const T m_finalValue;
    const T m_deltaValue;
    const double m_time;

    uint32_t m_currentStateIndex;
    const std::vector<State> m_states;

    void Update(const double p_elaspedTimeInSeconds) override
    {
        m_timeSinceChange += p_elaspedTimeInSeconds;

        const State& currentState = m_states[m_currentStateIndex];
        if (m_timeSinceChange < currentState.m_time)
        {
            return;
        }

        m_timeSinceChange = 0;
        m_currentStateIndex = (m_currentStateIndex + 1) % m_states.size();
    }
};*/

SimpleEngine::Counter::Counter(const std::vector<State> p_states)
    : m_type(Type::STATE), m_timeSinceChange(0), m_currentStateIndex(0), m_states(p_states)
{
    CounterService::Instance().RegisterForUpdates(this);
}

SimpleEngine::Counter::~Counter() { CounterService::Instance().UnregisterForUpdates(this); }

float SimpleEngine::Counter::GetFloatValueFromCurrentState() { return m_states[m_currentStateIndex].m_value; }

int32_t SimpleEngine::Counter::GetIntValueFromCurrentState()
{
    return static_cast<int32_t>(GetFloatValueFromCurrentState());
}

void SimpleEngine::Counter::Update(const double p_elaspedTimeInSeconds)
{
    m_timeSinceChange += p_elaspedTimeInSeconds;

    const State& currentState = m_states[m_currentStateIndex];
    if (m_timeSinceChange < currentState.m_time)
    {
        return;
    }

    m_timeSinceChange = 0;
    m_currentStateIndex = (m_currentStateIndex + 1) % m_states.size();
}
