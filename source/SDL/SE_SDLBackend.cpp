#include "SDL/SE_SDLBackend.h"

#include <stdexcept>
#include <string>

SimpleEngine::SE_Backend::SE_Backend(const std::string& a_windowTitle, uint16_t a_windowWidth, uint16_t a_windowHeight)
    : m_windowPtr(nullptr),
      m_windowTitle(a_windowTitle),
      m_windowWidth(a_windowWidth),
      m_windowHeight(a_windowHeight),
      m_isRunning(false) {}

SimpleEngine::SE_Backend::~SE_Backend() {}

void SimpleEngine::SE_Backend::Start() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::string sdlError = SDL_GetError();
        throw std::runtime_error("[SE_SDLBackend] Error initializing SDL backend: " + sdlError);
    }

    m_windowPtr = SDL_CreateWindow(m_windowTitle.c_str(),
                                   // TODO: pass these into options?
                                   100, 100, m_windowWidth, m_windowHeight, SDL_WINDOW_SHOWN);
    if (!m_windowPtr) {
        std::string sdlError = SDL_GetError();
        throw std::runtime_error("[SE_SDLBackend] Error creating SDL window: " + sdlError);
    }
    m_isRunning = true;
}

void SimpleEngine::SE_Backend::Update() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                m_isRunning = false;
                break;
            default:
                break;
        }
    }
}

void SimpleEngine::SE_Backend::Render() {}

void SimpleEngine::SE_Backend::Cleanup() {
    SDL_DestroyWindow(m_windowPtr);
    m_windowPtr = nullptr;
    SDL_Quit();
}