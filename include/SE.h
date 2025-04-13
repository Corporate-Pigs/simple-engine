#pragma once

#include <cstdint>
#include <string>

#define SE_SDL_BACKEND
#ifdef SE_SDL_BACKEND
#include "SDL/SE_sdlBackend.h"
#elif defined(SE_SMT_BACKEND)
#include "SE_NDSGraphics.h"
#else
#error "No backend selected!"
#endif

#include "SE_options.h"

namespace SimpleEngine {

class Game {
   public:
    Game(const Options& options);
    virtual ~Game();
    void Run();

   protected:
    Backend m_backend;

    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void Cleanup() = 0;

   private:
};

}  // namespace SimpleEngine
