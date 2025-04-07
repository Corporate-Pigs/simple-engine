#pragma once

#include <string>

#include "SE_Color.h"

namespace SimpleEngine {

struct SE_Sprite {
    const std::string k_asset;
    SE_Color color;
    SE_Sprite(const std::string& asset);
};

}  // namespace SimpleEngine
