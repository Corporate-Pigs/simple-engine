#include "SDL/SE_sdlGraphics.h"

#include <stb_image.h>

#include <cassert>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define SURFACE_MASKS 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
#else
#define SURFACE_MASKS 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000
#endif

// Public functions
void SimpleEngine::Graphics::DrawText() {}

void SimpleEngine::Graphics::DrawSprite(Sprite &p_spriteRef, const Transform &p_transform)
{
    SDL_Texture *texturePtr = GetSDLTextureForAssetName(p_spriteRef.c_asset);
    assert(texturePtr);

    uint32_t layerIndex = p_transform.m_layer % MAX_LAYERS;
    std::vector<RenderingUnit> &layerRef = m_layers[layerIndex];

    RenderingUnit ru = {RenderingUnitType::SPRITE, p_transform, texturePtr}; 
    layerRef.emplace_back(ru);
}

// Private functions
void SimpleEngine::Graphics::Start(SDL_Window *p_windowPtr)
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

    RenderLayers();

    SDL_RenderPresent(m_rendererPtr);
}

void SimpleEngine::Graphics::Cleanup()
{
    for (auto const &[key, mTexturePtr] : m_textureCache)
    {
        SDL_DestroyTexture(mTexturePtr);
    }
    SDL_DestroyRenderer(m_rendererPtr);
}

// Helper functions
SDL_Texture *SimpleEngine::Graphics::CreateSDLTexture(const std::string &p_assetPath)
{
    int width, height, channels;
    unsigned char *imageDataPtr = stbi_load(p_assetPath.c_str(), &width, &height, &channels, 4);
    if (!imageDataPtr)
    {
        throw std::runtime_error("[SDLGraphics] Error loading sprite: " + p_assetPath);
    }

    SDL_Surface *surfacePtr = SDL_CreateRGBSurfaceFrom(imageDataPtr, width, height, 32, width * 4, SURFACE_MASKS);
    if (!surfacePtr)
    {
        free(surfacePtr);
        throw std::runtime_error("[SDLGraphics] Error creating surface from asset: " + p_assetPath);
    }

    SDL_Texture *texturePtr = SDL_CreateTextureFromSurface(m_rendererPtr, surfacePtr);
    if (!texturePtr)
    {
        free(imageDataPtr);
        SDL_FreeSurface(surfacePtr);
        std::string error = SDL_GetError();
        throw std::runtime_error("[SDLGraphics] Error creating texture from asset: " + p_assetPath + ". " + error);
    }
    free(imageDataPtr);
    SDL_FreeSurface(surfacePtr);

    m_textureCache[p_assetPath] = texturePtr;

    return texturePtr;
}

SDL_Texture *SimpleEngine::Graphics::GetSDLTextureForAssetName(const std::string &p_assetPath)
{
    auto it = m_textureCache.find(p_assetPath);
    if (it != m_textureCache.end())
    {
        return it->second;
    }
    return CreateSDLTexture(p_assetPath);
}

void SimpleEngine::Graphics::RenderSprite(const RenderingUnit &p_renderingUnitRef)
{
    SDL_Texture *texturePtr = p_renderingUnitRef.m_item.m_texture;
    assert(texturePtr);

    int w, h;
    if (SDL_QueryTexture(texturePtr, NULL, NULL, &w, &h))
    {
        std::string error = SDL_GetError();
        throw std::runtime_error("[SDLGraphics] Error query texture: " + error);
    }

    const SDL_FRect screenRect = {p_renderingUnitRef.m_transform.m_position.x,
                                  p_renderingUnitRef.m_transform.m_position.y, 
                                  static_cast<float>(w),
                                  static_cast<float>(h)};

    if (SDL_RenderCopyExF(m_rendererPtr, texturePtr, NULL, &screenRect, p_renderingUnitRef.m_transform.m_rotation, NULL,
                          SDL_FLIP_NONE))
    {
        std::string error = SDL_GetError();
        throw std::runtime_error("[SDLGraphics] Error while render copy exF: " + error);
    };
}

void SimpleEngine::Graphics::RenderLayers()
{
    for (auto &layerRef : m_layers)
    {
        for (uint32_t i = 0; i < layerRef.size(); i++)
        {
            const uint32_t ruIndex = layerRef.size() - 1 - i;
            const RenderingUnit &renderingUnitRef = layerRef[ruIndex];

            switch (renderingUnitRef.m_type)
            {
                case RenderingUnitType::SPRITE:
                    RenderSprite(renderingUnitRef);
                    break;
                case RenderingUnitType::UNDEFINED:
                default:
                    assert(false);
            }
        }
        layerRef.clear();
    }
}
