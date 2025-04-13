#include <SDL.h>
#include <string>
//#include "SE_SDLGraphics.h"
//#include "SE_SDLUserInterface.h"

#pragma once

namespace SimpleEngine {

struct SE_Backend {
    friend class SimpleEngine;

   private:
    SDL_Window *m_windowPtr;
    const std::string m_windowTitle;
    const uint16_t m_windowWidth;
    const uint16_t m_windowHeight;

    void Start();
    void Update();
    void Render();
    void Cleanup();

   protected:
    //SE_Graphics m_graphics;
    //SE_UserInterface m_userInferface;
    bool m_isRunning;

   public:
    SE_Backend(const std::string& windowTitle, uint16_t windowWidth, uint16_t windowHeight);
    ~SE_Backend();

};

}  // namespace SimpleEngine
