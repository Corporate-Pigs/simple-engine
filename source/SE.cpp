#include "SE.h"

SimpleEngine::Game::Game(const Options& a_options)
    : m_backend(a_options.windowTitle, a_options.windowWidth, a_options.windowHeight) {}

SimpleEngine::Game::~Game() {


}

void SimpleEngine::Game::Run() {
    m_backend.Start();
    Start();
    while (m_backend.m_isRunning) {
        m_backend.Update();
        Update();
        m_backend.m_graphics.Render();
    }
    Cleanup();
}