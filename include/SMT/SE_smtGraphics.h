#pragma once

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
    Color m_backgroundColor;
    void DrawLabel(const Label &p_label, const Transform &p_transform);
    void DrawSprite(const Sprite &p_sprite, const Transform &p_transform);

   private:
    bool m_isRunning;
    bool m_wasInitiated;

    void Start();
    void Render();
    void Cleanup();
};

}  // namespace SimpleEngine
