#pragma once

#include <algorithm>
#include <cassert>
#include <cppvec/vec2.h>
#include <functional>

namespace SimpleEngine
{

template <typename T>
struct SpeedBasedLerp
{
    const T c_initialValue, c_finalValue;
    const double c_speed;

    T m_currentValue;

    SpeedBasedLerp(const T p_initialValue, const T p_finalValue, const double p_speed)
        : c_initialValue(p_initialValue),
          c_finalValue(p_finalValue),
          c_speed(p_speed),
          m_currentValue(p_initialValue)
        {};
};

template <typename T>
bool DidEnd(const SpeedBasedLerp<T> &p_animation)
{
    return p_animation.m_currentValue == p_animation.c_finalValue;
}

template <typename T>
void Reset(SpeedBasedLerp<T> &p_animation)
{
    p_animation.m_currentValue = p_animation.c_initialValue;
    p_animation.m_elapsedTime = 0.0;
}

template <typename T, typename DistanceFunction>
void UpdateAnimation(SpeedBasedLerp<T> &p_animation, const double p_elapsedTime, DistanceFunction p_distanceFunction)
{
    if (DidEnd(p_animation))
    {
        return;
    }
    
    const T delta = p_animation.c_finalValue - p_animation.m_currentValue;
    const float distance = p_distanceFunction(delta);
    assert(distance > 0);

    const float step = std::min(static_cast<float>(p_animation.c_speed * p_elapsedTime), distance);
    const T direction = delta / distance;

    p_animation.m_currentValue += direction * step;
}

}