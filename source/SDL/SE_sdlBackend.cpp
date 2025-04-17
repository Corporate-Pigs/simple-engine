#include "SDL/SE_sdlBackend.h"

#include <stdexcept>
#include <string>

SimpleEngine::Backend::Backend(const std::string &p_windowTitle, uint16_t p_windowWidth, uint16_t p_windowHeight)
    : m_windowPtr(nullptr),
      m_windowTitle(p_windowTitle),
      m_windowWidth(p_windowWidth),
      m_windowHeight(p_windowHeight),
      m_isRunning(false)
{
}

// Private Functions

void SimpleEngine::Backend::Start()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::string error = SDL_GetError();
        throw std::runtime_error("[SDLBackend] Error initializing SDL backend: " + error);
    }
    m_windowPtr = SDL_CreateWindow(m_windowTitle.c_str(),
                                   // TODO: pass these into options?
                                   100, 100, m_windowWidth, m_windowHeight, SDL_WINDOW_SHOWN);
    if (!m_windowPtr)
    {
        std::string error = SDL_GetError();
        throw std::runtime_error("[SDLBackend] Error creating SDL window: " + error);
    }

    m_graphics.Start(m_windowPtr);

    if (TTF_Init() == -1)
    {
        // handle error
    }

    m_isRunning = true;
}

void SimpleEngine::Backend::Update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
            case SDL_QUIT:
                m_isRunning = false;
                break;
            default:
                break;
        }
    }
}

void SimpleEngine::Backend::Cleanup()
{
    m_graphics.Cleanup();

    SDL_DestroyWindow(m_windowPtr);
    SDL_Quit();
    m_windowPtr = nullptr;
}
