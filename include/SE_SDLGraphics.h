#pragma once

#include <iostream>

#include "SE_Sprite.h"

namespace SimpleEngine {

class SE_Graphics {
    friend class SimpleEngine;

   private:
    static constexpr const char* Name() { return "SDL"; }

    void Initialize() {
        std::cout << "Initializing SDL..." << std::endl;
        // SDL initialization code
    }

    void Render() {
        std::cout << "Rendering with SDL..." << std::endl;
        // SDL rendering code
    }

    void Shutdown() {
        std::cout << "Shutting down SDL..." << std::endl;
        // SDL shutdown code
    }

   public:
    void DrawSprite(const SE_Sprite& sprite);
};

}  // namespace SimpleEngine
