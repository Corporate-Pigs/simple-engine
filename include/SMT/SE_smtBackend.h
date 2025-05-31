#pragma once

#include <string>

#include "SE_smtGraphics.h"
#include "SE_smtSound.h"
#include "SE_sprite.h"

namespace SimpleEngine
{

struct Backend
{
    friend class Game;

   public:
    Graphics m_graphics;
    Sound m_sound;
    const double UpdateElapsedTime();

   protected:
    bool m_isRunning;
    double m_elapsedTimeInSeconds;

   private:
    Backend(const std::string &p_windowTitle, uint16_t p_windowWidth, uint16_t p_windowHeight);

    const std::string m_windowTitle;
    const uint16_t m_windowWidth;
    const uint16_t m_windowHeight;
    uint32_t m_lastUpdateStart;

    void Start();
    void Update();
    void Cleanup();
};

}  // namespace SimpleEngine
