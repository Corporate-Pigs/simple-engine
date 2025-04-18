#pragma once

#include <string>

#include "SE_modifier.h"

namespace SimpleEngine
{

struct Sprite
{
    const std::string c_asset;
    std::vector<Modifier> m_modifiers;
};

}  // namespace SimpleEngine
