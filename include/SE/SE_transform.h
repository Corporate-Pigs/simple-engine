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
    float m_rotation;
};

}  // namespace SimpleEngine
