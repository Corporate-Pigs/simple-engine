#include "SDL/SE_sdlGraphics.h"

#include <stb_image.h>

void SimpleEngine::Graphics::DrawText() {}

void SimpleEngine::Graphics::DrawSprite(Sprite& p_sprite)
{
    int width, height, channels;
    unsigned char* data =
        stbi_load(p_sprite.c_asset.c_str(), &width, &height, &channels, 4);  // force 4 channels (RGBA)
    if (!data)
    {
        throw std::runtime_error("[SDLGraphics] Error loading sprite: " + p_sprite.c_asset);
    }

    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(data, width, height, 32, width * 4,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                                                    0xFF000000,  // Rmask
                                                    0x00FF0000,  // Gmask
                                                    0x0000FF00,  // Bmask
                                                    0x000000FF   // Amask
#else
                                                    0x000000FF,  // Rmask
                                                    0x0000FF00,  // Gmask
                                                    0x00FF0000,  // Bmask
                                                    0xFF000000   // Amask
#endif
    );
}

void SimpleEngine::Graphics::Start(SDL_Window* p_windowPtr)
{
    m_rendererPtr = SDL_CreateRenderer(p_windowPtr, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_rendererPtr)
    {
        std::string error = SDL_GetError();
        throw std::runtime_error("[SDLGraphics] Error initializing SDL renderer: " + error);
    }
}

void SimpleEngine::Graphics::Render()
{
    SDL_SetRenderDrawColor(m_rendererPtr, 0x55, 0x55, 0x55, 0xFF);
    SDL_RenderClear(m_rendererPtr);

    // render

    SDL_RenderPresent(m_rendererPtr);
}

void SimpleEngine::Graphics::Cleanup() { SDL_DestroyRenderer(m_rendererPtr); }
