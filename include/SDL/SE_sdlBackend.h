#pragma once

#include <SDL.h>

#include <string>

#include "SDL/SE_sdlGraphics.h"
#include "SDL/SE_sdlInput.h"
#include "SE_sprite.h"

namespace SimpleEngine
{

struct Backend
{
    friend class Game;

   public:
    Graphics m_graphics;

   protected:
    bool m_isRunning;

   private:
    Backend(const std::string &p_windowTitle, uint16_t p_windowWidth, uint16_t p_windowHeight);

    SDL_Window *m_windowPtr;
    const std::string m_windowTitle;
    const uint16_t m_windowWidth;
    const uint16_t m_windowHeight;

    void Start();
    void Update();
    void Render();
    void Cleanup();
};

}  // namespace SimpleEngine
