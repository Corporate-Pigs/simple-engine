#pragma once

#include <cstdint>
#include <string>

#define SE_SDL_GRAPHICS
#ifdef SE_SDL_GRAPHICS
#include "SE_SDLBackend.h"
#elif defined(SE_SMT_GRAPHICS)
#include "SMT/SE_SMTGraphics.h"
#else
#error "No backend selected!"
#endif

#include "SE_Options.h"

namespace SimpleEngine {

class SimpleEngine {
   public:
    SimpleEngine(const SE_Options& options);
    ~SimpleEngine();
    void Run();

   protected:
    SE_Backend m_backend;

    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void Cleanup() = 0;

   private:
};

}  // namespace SimpleEngine
