#pragma once

#include "SE_Sprite.h"

namespace SimpleEngine {

class SE_Graphics {
    friend class SimpleEngine;

   public:
    void DrawSprite(const SE_Sprite& sprite);

   private:
    void Initialize();
    void Render();
    void Cleanup();
};

}  // namespace SimpleEngine
