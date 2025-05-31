#include "SE.h"

#include <iostream>
#include <ostream>

SimpleEngine::Game::Game(const Options& a_options)
    : m_backend(a_options.windowTitle, a_options.windowWidth, a_options.windowHeight)
{
}

void SimpleEngine::Game::Run()
{
    if (m_backend.m_isRunning)
    {
        assert(false);
    }

    m_backend.Start();
    Start();

    double lagTime = 0.0;

    while (m_backend.m_isRunning)
    {
        double elaspedTime = m_backend.UpdateElapsedTime();
        lagTime += elaspedTime;

        m_backend.Update();
        uint32_t updates = 0;
        while (lagTime >= k_updateStep)
        {
            Update();
            lagTime -= k_updateStep;
            updates++;
        }
        
        //std::cout << elaspedTime << " : " << updates << std::endl;

        Render();
        m_backend.m_graphics.Render();
    }

    m_backend.Cleanup();
    Cleanup();
}