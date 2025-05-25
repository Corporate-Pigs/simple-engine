#pragma once

#include <vector>
#include <cppvec/vec2.h>

#include "SE_color.h"
#include "SE_modifier.h"

namespace SimpleEngine
{

struct Rectangle
{
    Color m_color;
    cppvec::Vec2<uint32_t> m_size;
    std::vector<Modifier> m_modifiers;
};

}  // namespace SimpleEngine
