#include "SE_sdlBackend.h"

#include <stdexcept>
#include <string>

SimpleEngine::Backend::Backend(const std::string &p_windowTitle, uint16_t p_windowWidth, uint16_t p_windowHeight)
    : m_windowPtr(nullptr),
      m_windowTitle(p_windowTitle),
      m_windowWidth(p_windowWidth),
      m_windowHeight(p_windowHeight),
      m_isRunning(false),
      m_elapsedTimeInSeconds(0),
      m_lastUpdateStart(0),
      m_graphics(p_windowWidth, p_windowHeight)
{
}

const double SimpleEngine::Backend::GetElapsedTime() { return m_elapsedTimeInSeconds; }

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

    m_sound.Start();
    m_graphics.Start(m_windowPtr);
    m_isRunning = true;
}

void SimpleEngine::Backend::Update()
{
    if (!m_isRunning)
    {
        return;
    }

    uint32_t currentTime = SDL_GetTicks();
    m_elapsedTimeInSeconds = (currentTime - m_lastUpdateStart) / 1000.0f;  // seconds
    m_lastUpdateStart = currentTime;

    // double fps = 1 / m_elapsedTimeInSeconds;
    // printf("%f\n", fps);

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
    m_sound.Cleanup();

    SDL_DestroyWindow(m_windowPtr);
    SDL_Quit();
    m_windowPtr = nullptr;
}