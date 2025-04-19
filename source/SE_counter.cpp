
#include "SE_counter.h"

#include "SE_counterService.h"

SimpleEngine::Counter::Counter(const std::vector<State> p_states, const bool p_loop, const bool p_active)
    : m_type(Type::STATE),
      m_timeSinceChange(0),
      m_currentStateIndex(0),
      m_states(p_states),
      m_done(false),
      m_active(p_active),
      m_loop(p_loop),
      m_initialValue(0),
      m_finalValue(0),
      m_deltaValue(0),
      m_currentValue(0),
      m_time(0)
{
    CounterService::Instance().RegisterForUpdates(this);
}

SimpleEngine::Counter::Counter(const float p_initialValue, const float p_finalValue, const float p_deltaValue,
                               const float p_interval, const bool p_loop, const bool p_active)
    : m_type(Type::DELTA),
      m_timeSinceChange(0),
      m_currentStateIndex(0),
      m_done(false),
      m_active(p_active),
      m_loop(p_loop),
      m_initialValue(p_initialValue),
      m_finalValue(p_finalValue),
      m_deltaValue(p_deltaValue),
      m_currentValue(p_initialValue),
      m_time(p_interval)
{
    CounterService::Instance().RegisterForUpdates(this);
}

SimpleEngine::Counter::Counter(const float p_initialValue, const float p_finalValue, const float p_interval,
                               const bool p_loop, const bool p_active)
    : m_type(Type::TIME),
      m_timeSinceChange(0),
      m_currentStateIndex(0),
      m_done(false),
      m_active(p_active),
      m_loop(p_loop),
      m_initialValue(p_initialValue),
      m_finalValue(p_finalValue),
      m_deltaValue((p_initialValue - p_finalValue) / p_interval),
      m_currentValue(p_initialValue),
      m_time((p_initialValue - p_finalValue) / m_deltaValue)
{
    CounterService::Instance().RegisterForUpdates(this);
}

SimpleEngine::Counter::~Counter() { CounterService::Instance().UnregisterForUpdates(this); }

float SimpleEngine::Counter::GetFloatValueFromCurrentState()
{
    switch (m_type)
    {
        case Counter::Type::STATE:
            return m_states[m_currentStateIndex].m_value;
        case Counter::Type::DELTA:
        case Counter::Type::TIME:
            return m_currentValue;
        case Counter::Type::UNDEFINED:
        default:
            assert(false);
    }
}

int32_t SimpleEngine::Counter::GetIntValueFromCurrentState()
{
    return static_cast<int32_t>(GetFloatValueFromCurrentState());
}

void SimpleEngine::Counter::Reset()
{
    m_done = false;
    m_currentStateIndex = 0;
    m_timeSinceChange = 0;
    m_currentValue = m_initialValue;
}

void SimpleEngine::Counter::Update(const double p_elaspedTimeInSeconds)
{
    if (!m_active || m_done)
    {
        return;
    }

    m_timeSinceChange += p_elaspedTimeInSeconds;

    switch (m_type)
    {
        case Counter::Type::STATE:
            UpdateState();
            break;
        case Counter::Type::DELTA:
        case Counter::Type::TIME:
            UpdateDelta();
            break;
        case Counter::Type::UNDEFINED:
        default:
            assert(false);
    }
}

void SimpleEngine::Counter::UpdateState()
{
    const State &currentState = m_states[m_currentStateIndex];
    if (m_timeSinceChange < currentState.m_time)
    {
        return;
    }

    m_timeSinceChange = 0;
    m_currentStateIndex++;

    if (m_currentStateIndex == m_states.size())
    {
        if (m_loop)
        {
            m_currentStateIndex %= m_states.size();
            return;
        }
        m_done = true;
    }
}

void SimpleEngine::Counter::UpdateDelta()
{
    if (m_timeSinceChange < m_time)
    {
        return;
    }
    m_timeSinceChange = 0;
    m_currentValue += m_deltaValue;

    bool isInfinite =
        (m_initialValue < m_finalValue && m_deltaValue < 0) || (m_initialValue > m_finalValue && m_deltaValue > 0);
    if (isInfinite)
    {
        return;
    }

    bool reachedEnd =
        m_deltaValue > 0 && m_currentValue >= m_finalValue || m_deltaValue < 0 && m_currentValue <= m_finalValue;

    if (reachedEnd)
    {
        if (m_loop)
        {
            m_currentValue = m_initialValue;
            return;
        }
        m_done = true;
    }
}
