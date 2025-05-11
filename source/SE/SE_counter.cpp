
#include "SE_counter.h"

#include <cassert>

#include "SE_counterService.h"

SimpleEngine::Counter::Counter()
    : m_type(Type::UNDEFINED),
      m_timeSinceChange(0),
      m_currentStateIndex(0),
      m_states({}),
      m_done(false),
      m_active(false),
      m_loop(false),
      m_initialValue(0),
      m_finalValue(0),
      m_deltaValue(0),
      m_currentValue(0),
      m_currentValuePtr(nullptr),
      m_time(0)
{
    m_isRegistered = false;
}

SimpleEngine::Counter::Counter(uint32_t *p_currentValuePtr, const std::vector<State> p_states, const bool p_loop,
                               const bool p_active)
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
      m_currentValuePtr(p_currentValuePtr),
      m_time(0)
{
    CounterService::Instance().RegisterForUpdates(this);
    m_isRegistered = true;
}

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
      m_currentValuePtr(nullptr),
      m_currentStateIndexPtr(nullptr),
      m_time(0)
{
    CounterService::Instance().RegisterForUpdates(this);
    m_isRegistered = true;
}

SimpleEngine::Counter::Counter(const int32_t p_initialValue, const int32_t p_finalValue, const int32_t p_deltaValue,
                               const double p_interval, const bool p_loop, const bool p_active)
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
      m_currentValuePtr(nullptr),
      m_currentStateIndexPtr(nullptr),
      m_time(p_interval)
{
    CounterService::Instance().RegisterForUpdates(this);
    m_isRegistered = true;
}

SimpleEngine::Counter::Counter(const int32_t p_initialValue, const int32_t p_finalValue, const double p_interval,
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
      m_currentValuePtr(nullptr),
      m_currentStateIndexPtr(nullptr),
      m_time((p_initialValue - p_finalValue) / m_deltaValue)
{
    CounterService::Instance().RegisterForUpdates(this);
    m_isRegistered = true;
}

SimpleEngine::Counter::~Counter()
{
    if (!m_isRegistered)
    {
        return;
    }
    CounterService::Instance().UnregisterForUpdates(this);
}

int32_t SimpleEngine::Counter::GetIntValueFromCurrentState()
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
    
    uint32_t *currentValuePtr = &m_currentValue;
    if (m_currentValuePtr != nullptr)
    {
        currentValuePtr = m_currentValuePtr;
    }

    *currentValuePtr = currentState.m_value;

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
