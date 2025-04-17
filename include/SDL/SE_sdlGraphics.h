#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "SE_sprite.h"
#include "SE_transform.h"

#define MAX_LAYERS 32

namespace SimpleEngine
{

    struct Graphics
    {
        friend class Game;
        friend class Backend;

    public:
        void DrawText();
        void DrawSprite(Sprite &p_spritePtr, const Transform &p_transform);

    private:
        enum class RenderingUnitType
        {
            UNDEFINED = 0,
            SPRITE
        };

        struct RenderingUnit
        {
            RenderingUnitType m_type;
            Transform m_transform;
            union RenderingItem
            {
                SDL_Texture *m_texture;

            } m_item;
        };

        bool m_isRunning;
        SDL_Renderer *m_rendererPtr;
        TTF_Font *m_fontPtr; 
        std::unordered_map<std::string, SDL_Texture *> m_textureCache;
        std::vector<RenderingUnit> m_layers[MAX_LAYERS];

        void Start(SDL_Window *p_windowPtr);
        void Render();
        void Cleanup();

        SDL_Texture *CreateSDLTexture(const std::string &p_assetPath);
        SDL_Texture *GetSDLTextureForAssetName(const std::string &p_assetPath);
        void RenderSprite(const RenderingUnit &p_renderingUnitRef);
        void RenderLayers();
    };

} // namespace SimpleEngine
