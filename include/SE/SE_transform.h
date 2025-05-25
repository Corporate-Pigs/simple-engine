#pragma once

#include <cppvec/vec2.h>

#include <cinttypes>

namespace SimpleEngine
{

struct Transform
{
    uint32_t m_layer;
    cppvec::Vec2<float> m_position;
    cppvec::Vec2<float> m_scale;
    double m_rotationInRadians;
    cppvec::Vec2<float> m_anchor;

    void RotateAroundPoint(const cppvec::Vec2<float>& p_point, const float p_radians);
};

}  // namespace SimpleEngine
