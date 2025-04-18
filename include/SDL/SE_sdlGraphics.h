#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "SE_label.h"
#include "SE_sprite.h"
#include "SE_transform.h"

static constexpr uint32_t k_maxLayers = 32;
static constexpr uint32_t k_defaultFontPtSize = 100;

namespace SimpleEngine
{

struct Graphics
{
    friend class Game;
    friend class Backend;

   public:
    void DrawLabel(const Label &p_label, const Transform &p_transform);
    void DrawSprite(const Sprite &p_sprite, const Transform &p_transform);

   private:
    enum class RenderingUnitType
    {
        UNDEFINED = 0,
        SPRITE,
        LABEL
    };

    struct RenderingUnit
    {
        const RenderingUnitType m_type;
        const Transform m_transform;
        const union RenderingItem
        {
            SDL_Texture *m_texture;

        } m_item;
    };

    bool m_isRunning;
    bool m_wasInitiated;
    SDL_Renderer *m_rendererPtr;
    std::unordered_map<std::string, SDL_Texture *> m_textureCache;
    std::unordered_map<std::string, TTF_Font *> m_fontCache;
    std::vector<RenderingUnit> m_layers[k_maxLayers];

    void Start(SDL_Window *p_windowPtr);
    void Render();
    void Cleanup();

    // Helper functions
    SDL_Texture *CreateSDLTexture(const std::string &p_assetPath);
    SDL_Texture *GetSDLTextureForAssetName(const std::string &p_assetPath);

    TTF_Font *CreateTTFFont(const std::string &p_assetPath, const uint32_t p_ptsize);
    TTF_Font *GetTTFFontForAssetName(const std::string &p_assetPath);

    void RenderSDLTexture(SDL_Texture *p_texturePtr, const Transform &p_transform);
    void RenderSprite(const RenderingUnit &p_renderingUnitRef);
    void RenderLabel(const RenderingUnit &p_renderingUnitRef);
    void RenderLayers();
};

}  // namespace SimpleEngine
