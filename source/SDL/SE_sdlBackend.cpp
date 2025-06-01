#include "SE_sdlBackend.h"

#include <stdexcept>
#include <string>

SimpleEngine::Backend::Backend(const std::string &p_windowTitle, uint16_t p_windowWidth, uint16_t p_windowHeight)
    : m_windowPtr(nullptr),
      m_baseWindowTitle(p_windowTitle),
      m_windowWidth(p_windowWidth),
      m_windowHeight(p_windowHeight),
      m_isRunning(false),
      m_elapsedTimeInSeconds(0),
      m_lastUpdateStart(0),
      m_graphics(p_windowWidth, p_windowHeight),
      m_framesPerSecond(0),
      m_input()
{
}

const double SimpleEngine::Backend::UpdateElapsedTime()
{
    uint32_t currentTime = SDL_GetTicks();
    m_elapsedTimeInSeconds = (currentTime - m_lastUpdateStart) / 1000.0f;
    m_lastUpdateStart = currentTime;
    m_framesPerSecond = 1 / m_elapsedTimeInSeconds;

    UpdateWindowTitle(currentTime);
    return m_elapsedTimeInSeconds;
}

uint32_t SimpleEngine::Backend::GetUptime()
{
    return SDL_GetTicks() / 1000.0f;
}

// Private Functions
void SimpleEngine::Backend::Start()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::string error = SDL_GetError();
        throw std::runtime_error("[SDLBackend] Error initializing SDL backend: " + error);
    }
    m_windowPtr = SDL_CreateWindow(m_baseWindowTitle.c_str(),
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

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
            case SDL_QUIT:
                m_isRunning = false;
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEWHEEL:
                m_input.Update(event, m_elapsedTimeInSeconds);
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

void SimpleEngine::Backend::UpdateWindowTitle(uint32_t currentTime)
{
    m_timeSinceTitleUpdate += m_elapsedTimeInSeconds;
    if (m_timeSinceTitleUpdate >= 1)
    {
        std::string windowTitle = m_baseWindowTitle + " - fps:" + std::to_string(m_framesPerSecond);
        SDL_SetWindowTitle(m_windowPtr, windowTitle.c_str());
        m_timeSinceTitleUpdate = 0;
    }
}
