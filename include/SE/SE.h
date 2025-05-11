#pragma once

#include <cassert>
#include <cstdint>
#include <string>

#ifdef SE_SDL_BACKEND
#include "SE_sdlBackend.h"
#elif defined(SE_SMT_BACKEND)
#include "SE_smtBackend.h"
#else
#error "No backend selected!"
#endif

#include "SE_options.h"

namespace SimpleEngine
{

class Game
{
   public:
    Game(const Options& options);
    virtual ~Game();
    void Run();

   protected:
    Backend m_backend;

    virtual void Start() = 0;
    virtual void Update(const double p_elapsedTimeInSeconds) = 0;
    virtual void Render() = 0;
    virtual void Cleanup() = 0;

   private:
};

}  // namespace SimpleEngine
