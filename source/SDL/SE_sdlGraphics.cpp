#include "SE_sdlGraphics.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <cassert>
#include <stdexcept>

// Public functions
SimpleEngine::Graphics::Graphics(const uint16_t p_windowWidth, const uint16_t p_windowHeight)
    : m_backgroundColor({0, 0, 0, 255}),
      m_camera({0, {0.0f, 0.0f}, {static_cast<float>(p_windowWidth), static_cast<float>(p_windowHeight)}, 0.0f})
{
}

void SimpleEngine::Graphics::DrawLabel(const Label &p_label, const Transform &p_transform)
{
    TTF_Font *fontPtr = GetTTFFontForAssetName(p_label.c_font_asset);
    assert(fontPtr);

    SDL_Surface *surfacePtr = TTF_RenderText_Blended(
        fontPtr, p_label.m_text.c_str(), {p_label.m_color.r, p_label.m_color.g, p_label.m_color.b, p_label.m_color.a});

    SDL_Texture *texturePtr = SDL_CreateTextureFromSurface(m_rendererPtr, surfacePtr);
    SDL_FreeSurface(surfacePtr);

    uint32_t layerIndex = p_transform.m_layer % k_maxLayers;
    std::vector<RenderingUnit> &layerRef = m_layers[layerIndex];

    RenderingUnit ru = {RenderingUnit::Type::LABEL,
                        {p_transform.m_layer, p_transform.m_position,
                         p_transform.m_scale * (static_cast<float>(p_label.c_font_ptsize) / k_defaultFontPtSize),
                         p_transform.m_rotationInRadians},
                        texturePtr};
    layerRef.emplace_back(ru);
}

void SimpleEngine::Graphics::DrawSprite(const Sprite &p_sprite, const Transform &p_transform)
{
    SDL_Texture *texturePtr = GetSDLTextureForAssetName(p_sprite.c_asset);
    assert(texturePtr);

    uint32_t layerIndex = p_transform.m_layer % k_maxLayers;
    std::vector<RenderingUnit> &layerRef = m_layers[layerIndex];

    RenderingUnit ru = {RenderingUnit::Type::SPRITE, p_transform, texturePtr, p_sprite.m_modifiers};
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
    SDL_SetRenderDrawColor(m_rendererPtr, m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b,
                           m_backgroundColor.a);
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
    return CreateTTFFont(p_assetPath, k_defaultFontPtSize);
}

SDL_FRect SimpleEngine::Graphics::ComputeScreenRectForRenderingUnit(const RenderingUnit &p_renderingUnitRef)
{
    SDL_Texture *texturePtr = p_renderingUnitRef.m_item.m_texture;
    assert(texturePtr);

    int w, h;
    if (SDL_QueryTexture(texturePtr, NULL, NULL, &w, &h))
    {
        std::string error = SDL_GetError();
        throw std::runtime_error("[SDLGraphics] Error query texture: " + error);
    }

    Transform t = p_renderingUnitRef.m_transform;
    t.RotateAroundPoint({m_camera.m_position + (m_camera.m_scale * 0.5f)}, m_camera.m_rotationInRadians);

    t.m_position -= m_camera.m_position;
    t.m_scale.x *= w;
    t.m_scale.y *= h;

    return {t.m_position.x, t.m_position.y, t.m_scale.x, t.m_scale.y};
}

void SimpleEngine::Graphics::RenderSprite(const RenderingUnit &p_renderingUnitRef)
{
    SDL_Texture *texturePtr = p_renderingUnitRef.m_item.m_texture;
    assert(texturePtr);

    SDL_FRect screenRect = ComputeScreenRectForRenderingUnit(p_renderingUnitRef);
    SDL_Rect *atlasRectPtr = NULL;
    SDL_Rect atlasRect;

    SDL_SetTextureColorMod(texturePtr, 255, 255, 255);
    SDL_SetTextureAlphaMod(texturePtr, 255);

    for (const auto modifier : p_renderingUnitRef.m_modifiers)
    {
        switch (modifier.m_type)
        {
            case Modifier::Type::ATLAS:
            {
                int tw = screenRect.w / p_renderingUnitRef.m_transform.m_scale.x / modifier.m_atlas.m_numberOfColumns;
                int th = screenRect.h / p_renderingUnitRef.m_transform.m_scale.y / modifier.m_atlas.m_numberOfRows;
                int tx = modifier.m_atlas.m_spriteIndex % modifier.m_atlas.m_numberOfColumns * tw;
                int ty = modifier.m_atlas.m_spriteIndex / modifier.m_atlas.m_numberOfColumns * th;
                atlasRect = {tx, ty, tw, th};
                atlasRectPtr = &atlasRect;
                screenRect.w = tw * p_renderingUnitRef.m_transform.m_scale.x;
                screenRect.h = th * p_renderingUnitRef.m_transform.m_scale.y;
                break;
            }
            case Modifier::Type::COLOR:
            {
                SDL_SetTextureColorMod(texturePtr, modifier.m_color.m_color.r, modifier.m_color.m_color.g,
                                       modifier.m_color.m_color.b);
                SDL_BlendMode blendMode = SDL_BLENDMODE_NONE;
                switch (modifier.m_color.m_type)
                {
                    case ColorModifierData::Type::ADD:
                        blendMode = SDL_BLENDMODE_ADD;
                        break;
                    case ColorModifierData::Type::BLEND:
                        blendMode = SDL_BLENDMODE_BLEND;
                        break;
                    case ColorModifierData::Type::MOD:
                        blendMode = SDL_BLENDMODE_MOD;
                        break;
                    case ColorModifierData::Type::MUL:
                        blendMode = SDL_BLENDMODE_MUL;
                        break;
                    case ColorModifierData::Type::UNDEFINED:
                    default:
                        assert(false);
                }
                SDL_SetTextureBlendMode(texturePtr, blendMode);
                break;
            }
            case Modifier::Type::ALPHA:
            {
                SDL_SetTextureAlphaMod(texturePtr, modifier.m_alpha);
                break;
            }
            case Modifier::Type::UNDEFINED:
            default:
                assert(false);
        }
    }

    screenRect.x -= screenRect.w * p_renderingUnitRef.m_transform.m_anchor.x;
    screenRect.y -= screenRect.h * p_renderingUnitRef.m_transform.m_anchor.y;

    double screenRectRotationDegrees = p_renderingUnitRef.m_transform.m_rotationInRadians + m_camera.m_rotationInRadians * (180.0 / M_PI);
    if (SDL_RenderCopyExF(m_rendererPtr, texturePtr, atlasRectPtr, &screenRect, screenRectRotationDegrees, NULL,
                          SDL_FLIP_NONE))
    {
        std::string error = SDL_GetError();
        throw std::runtime_error("[SDLGraphics] Error while render copy exF: " + error);
    };
}

void SimpleEngine::Graphics::RenderLabel(const RenderingUnit &p_renderingUnitRef)
{
    RenderSprite(p_renderingUnitRef);
    SDL_DestroyTexture(p_renderingUnitRef.m_item.m_texture);
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
                case RenderingUnit::Type::SPRITE:
                    RenderSprite(renderingUnitRef);
                    break;
                case RenderingUnit::Type::LABEL:
                    RenderLabel(renderingUnitRef);
                    break;
                case RenderingUnit::Type::UNDEFINED:
                default:
                    assert(false);
            }
        }
        layerRef.clear();
    }
}
