#pragma once

#include <cinttypes>
#include <string>

#include "SE_Sprite.h"

namespace SimpleEngine {

void SE_Init(const std::string& windowTitle, const uint32_t windowWidth, const uint32_t windowHeight);

void SE_Destroy();

void SE_Update();

bool SE_IsClosing();

uint64_t SE_GetTicks();

void SE_DrawSprite(const SE_Sprite& sprite);

}  // namespace SimpleEngine