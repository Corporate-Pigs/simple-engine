#pragma once

#include <cstdint>
#include <string>

#define SE_SMT_GRAPHICS

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

   protected:
    SE_Graphics m_graphics;
    //SE_Input m_input;
    double m_elapsedTime;

    uint64_t GetTicks();

    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void Terminate() = 0;

   public:
    SimpleEngine(const std::string& windowTitle, const uint32_t windowWidth, const uint32_t windowHeight);
    ~SimpleEngine();
    void Run();
};

}  // namespace SimpleEngine
