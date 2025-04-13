#pragma once

#include <SDL.h>

#include <stdexcept>
#include <string>

#include "SE_sprite.h"

namespace SimpleEngine
{

struct Graphics
{
    friend class Game;
    friend class Backend;

   public:
    void DrawText();
    void DrawSprite(Sprite &p_sprite);

   protected:
    SDL_Window *m_windowPtr;
    SDL_Renderer *m_rendererPtr;
    bool m_isRunning;

   private:
    void Start(SDL_Window *p_windowPtr);
    void Render();
    void Cleanup();
};

}  // namespace SimpleEngine
