#include "SDL/SE_sdlGraphics.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <cassert>

// Public functions
void SimpleEngine::Graphics::DrawLabel(const Label &p_label, const Transform &p_transform)
{
    TTF_Font *fontPtr = GetTTFFontForAssetName(p_label.c_font_asset);
    assert(fontPtr);

    SDL_Surface *surfacePtr = TTF_RenderText_Blended(
        fontPtr, 
        p_label.m_text.c_str(), 
        { p_label.m_color.r, p_label.m_color.g, p_label.m_color.b, p_label.m_color.a });

    SDL_Texture *texturePtr = SDL_CreateTextureFromSurface(m_rendererPtr, surfacePtr);
    SDL_FreeSurface(surfacePtr);

    uint32_t layerIndex = p_transform.m_layer % k_maxLayers;
    std::vector<RenderingUnit> &layerRef = m_layers[layerIndex];

    RenderingUnit ru = {RenderingUnitType::SPRITE, p_transform, texturePtr};
    layerRef.emplace_back(ru);
}

void SimpleEngine::Graphics::DrawSprite(const Sprite &p_sprite, const Transform &p_transform)
{
    SDL_Texture *texturePtr = GetSDLTextureForAssetName(p_sprite.c_asset);
    assert(texturePtr);

    uint32_t layerIndex = p_transform.m_layer % k_maxLayers;
    std::vector<RenderingUnit> &layerRef = m_layers[layerIndex];

    RenderingUnit ru = {RenderingUnitType::SPRITE, p_transform, texturePtr};
    layerRef.emplace_back(ru);
}

void SimpleEngine::Graphics::Start(SDL_Window *p_windowPtr)
{
    m_rendererPtr = SDL_CreateRenderer(p_windowPtr, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_rendererPtr)
    {
        std::string error = SDL_GetError();
        throw std::runtime_error("[SDLGraphics] Error initializing SDL renderer: " + error);
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::string error = IMG_GetError();
        throw std::runtime_error("[SDLGraphics] Error initializing IMG: " + error);
    }

    if (TTF_Init() == -1)
    {
        std::string error = TTF_GetError();
        throw std::runtime_error("[SDLGraphics] Error initializing TTF: " + error);
    }
    m_wasInitiated = true;
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
    if (!m_wasInitiated)
    {
        return;
    }

    for (auto const &[key, m_fontPtr] : m_fontCache)
    {
        TTF_CloseFont(m_fontPtr);
    }

    for (auto const &[key, mTexturePtr] : m_textureCache)
    {
        SDL_DestroyTexture(mTexturePtr);
    }

    SDL_DestroyRenderer(m_rendererPtr);
    IMG_Quit();
    TTF_Quit();
}

// Helper functions
SDL_Texture *SimpleEngine::Graphics::CreateSDLTexture(const std::string &p_assetPath)
{
    SDL_Surface *surfacePtr = IMG_Load(p_assetPath.c_str());
    if (!surfacePtr)
    {
        std::string error = IMG_GetError();
        throw std::runtime_error("[SDLGraphics] Error creating surface from asset: " + p_assetPath + ". " + error);
    }

    SDL_Texture *texturePtr = SDL_CreateTextureFromSurface(m_rendererPtr, surfacePtr);
    if (!texturePtr)
    {
        SDL_FreeSurface(surfacePtr);
        std::string error = SDL_GetError();
        throw std::runtime_error("[SDLGraphics] Error creating texture from asset: " + p_assetPath + ". " + error);
    }
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

TTF_Font *SimpleEngine::Graphics::CreateTTFFont(const std::string &p_assetPath, const uint32_t p_ptsize)
{
    TTF_Font *fontPtr = TTF_OpenFont(p_assetPath.c_str(), p_ptsize);
    if (!fontPtr)
    {
        std::string error = IMG_GetError();
        throw std::runtime_error("[SDLGraphics] Error creating font from asset: " + p_assetPath + ". " + error);
    }
    m_fontCache[p_assetPath] = fontPtr;
    return fontPtr;
}

TTF_Font *SimpleEngine::Graphics::GetTTFFontForAssetName(const std::string &p_assetPath)
{
    auto it = m_fontCache.find(p_assetPath);
    if (it != m_fontCache.end())
    {
        return it->second;
    }
    return CreateTTFFont(p_assetPath, 32);
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
                                  p_renderingUnitRef.m_transform.m_position.y, static_cast<float>(w),
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
