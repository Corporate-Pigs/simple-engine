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
    virtual ~Game() = default;
    void Run();

    Backend m_backend;

   protected:
    static constexpr double k_updateStep = 0.001;

    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void Cleanup() = 0;

   private:
};

}  // namespace SimpleEngine
