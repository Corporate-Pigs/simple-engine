#include "SE.h"

SimpleEngine::Game::Game(const Options& a_options)
    : m_backend(a_options.windowTitle, a_options.windowWidth, a_options.windowHeight)
{
}

SimpleEngine::Game::~Game() {}

void SimpleEngine::Game::Run()
{
    m_backend.Start();
    Start();

    double lagTime = 0.0;

    while (m_backend.m_isRunning)
    {
        m_backend.Update();

        lagTime += m_backend.GetElapsedTime();
        while (lagTime >= k_updateStep) {
            Update();
            lagTime -= k_updateStep;
        }
        Render();
        m_backend.m_graphics.Render();
    }

    m_backend.Cleanup();
    Cleanup();
}