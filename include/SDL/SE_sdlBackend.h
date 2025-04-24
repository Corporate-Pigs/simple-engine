#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>

#include "SDL/SE_sdlGraphics.h"
#include "SDL/SE_sdlInput.h"
#include "SDL/SE_sdlSound.h"
#include "SE_sprite.h"

namespace SimpleEngine
{

struct Backend
{
    friend class Game;

   public:
    Graphics m_graphics;
    Sound m_sound;
    const double GetElapsedTime();

   protected:
    bool m_isRunning;
    double m_elapsedTimeInSeconds;

   private:
    Backend(const std::string &p_windowTitle, uint16_t p_windowWidth, uint16_t p_windowHeight);

    SDL_Window *m_windowPtr;
    const std::string m_windowTitle;
    const uint16_t m_windowWidth;
    const uint16_t m_windowHeight;
    uint32_t m_lastUpdateStart;

    void Start();
    void Update();
    void Cleanup();
};

}  // namespace SimpleEngine
