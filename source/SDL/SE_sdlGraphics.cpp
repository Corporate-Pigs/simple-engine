#include "SDL/SE_sdlGraphics.h"

#include <SDL_image.h>

void SimpleEngine::Graphics::DrawText() {}

void SimpleEngine::Graphics::DrawSprite(Sprite& p_sprite) {}

void SimpleEngine::Graphics::Start(SDL_Window* p_windowPtr)
{
    m_rendererPtr = SDL_CreateRenderer(m_windowPtr, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_rendererPtr)
    {
        std::string error = SDL_GetError();
        throw std::runtime_error("[SDLBackend] Error initializing SDL renderer: " + error);
    }

    uint8_t imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        std::string error = SDL_GetError();
        throw std::runtime_error("[SDLBackend] Error initializing SDL IMG: " + error);
    }
}

void SimpleEngine::Graphics::Render() {
    SDL_SetRenderDrawColor(m_rendererPtr, 0x55, 0x55, 0x55, 0xFF);
    SDL_RenderClear(m_rendererPtr);

    // render 

    SDL_RenderPresent(m_rendererPtr);
}

void SimpleEngine::Graphics::Cleanup() {
    SDL_DestroyRenderer(m_rendererPtr);
    IMG_Quit();
}
