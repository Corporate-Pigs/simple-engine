#pragma once

#include <cppvec/vec2.h>

namespace SimpleEngine
{

template <typename T>
struct TimeBasedLerp
{
    const T c_initialValue, c_finalValue;
    const double c_duration;

    T m_currentValue;
    double m_elapsedTime;

    TimeBasedLerp(const T p_initialValue, const T p_finalValue, const double p_duration)
        : c_initialValue(p_initialValue),
          c_finalValue(p_finalValue),
          c_duration(p_duration),
          m_currentValue(p_initialValue),
          m_elapsedTime(0) {};
};

template <typename T>
bool DidEnd(const TimeBasedLerp<T> &p_animation)
{
    return p_animation.m_currentValue == p_animation.c_finalValue;
}

template <typename T>
void Reset(TimeBasedLerp<T> &p_animation)
{
    p_animation.m_currentValue = p_animation.c_initialValue;
    p_animation.m_elapsedTime = 0.0;
}

template <typename T>
void UpdateAnimation(TimeBasedLerp<T> &p_animation, const double p_elapsedTime)
{
    if (DidEnd(p_animation))
    {
        return;
    }
    p_animation.m_elapsedTime += p_elapsedTime;
    float percentage = static_cast<float>(p_animation.m_elapsedTime / p_animation.c_duration);
    p_animation.m_currentValue =
        (p_animation.c_initialValue * (1.0f - percentage)) + (p_animation.c_finalValue * percentage);
}

}