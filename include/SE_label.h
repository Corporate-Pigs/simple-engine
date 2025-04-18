#pragma once

#include <stdint.h>

#include <string>

#include "SE_color.h"

namespace SimpleEngine
{

struct Label
{
    const std::string c_font_asset;
    const uint8_t c_font_ptsize;
    std::string m_text;
    Color m_color;
};

}  // namespace SimpleEngine
