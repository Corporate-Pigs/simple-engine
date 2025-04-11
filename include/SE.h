#pragma once

#include <cstdint>
#include <string>

#ifndef SE_SDL_GRAPHICS
#ifndef SE_SMT_GRAPHICS
#define SE_SDL_GRAPHICS
#endif
#endif

#ifdef SE_SDL_GRAPHICS
    #include "SE_SDLGraphics.h"
#elif defined(SE_SMT_GRAPHICS)
    #include "SE_SMTGraphics.h"
#else
    #error "No backend selected!"
#endif

namespace SimpleEngine {

class SimpleEngine {
   private:
    void SE_Update();
    void SE_Draw();

   protected:
    SE_Graphics m_graphics;
    double m_elapsedTime;

    void DrawSprite();
    uint64_t GetTicks();

    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void End() = 0;

   public:
    SimpleEngine(const std::string& windowTitle, const uint32_t windowWidth, const uint32_t windowHeight);
    ~SimpleEngine();
};

}  // namespace SimpleEngine
