#include "SE.h"

SimpleEngine::Game::Game(const Options& a_options)
    : m_backend(a_options.windowTitle, a_options.windowWidth, a_options.windowHeight)
{
}

void SimpleEngine::Game::Run()
{
    if(m_backend.m_isRunning) {
        assert(false);
    }

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