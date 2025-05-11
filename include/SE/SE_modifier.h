#pragma once

#include <cinttypes>

#include "SE_color.h"

namespace SimpleEngine
{

struct AtlasModifierData
{
    uint32_t m_numberOfRows;
    uint32_t m_numberOfColumns;
    uint32_t m_spriteIndex;
};

struct ColorModifierData
{
    Color m_color;
    enum class Type
    {
        UNDEFINED = 0,
        BLEND,
        ADD,
        MOD,
        MUL,
    } m_type;
};

struct FlipModifierData
{
    bool m_horizontal;
    bool m_vertical;
};

struct Modifier
{
    const enum class Type { UNDEFINED = 0, ATLAS, COLOR, ALPHA, FLIP } m_type;

    const union
    {
        AtlasModifierData m_atlas;
        ColorModifierData m_color;
        FlipModifierData m_flip;
        float m_alpha;
    };
};

}  // namespace SimpleEngine
