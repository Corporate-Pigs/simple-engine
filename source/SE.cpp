#include "SE.h"

SimpleEngine::SimpleEngine::SimpleEngine(const SE_Options& a_options)
    : m_backend(a_options.windowTitle, a_options.windowWidth, a_options.windowHeight) {}

SimpleEngine::SimpleEngine::~SimpleEngine() {}

void SimpleEngine::SimpleEngine::Run() {
    m_backend.Start();
    Start();
    while (m_backend.m_isRunning) {
        m_backend.Update();
        Update();
        m_backend.Render();
    }
    Cleanup();
}