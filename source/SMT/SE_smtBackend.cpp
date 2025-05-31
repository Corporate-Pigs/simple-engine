#include "SE_smtBackend.h"

#include <smt/smt.h>
#include <smt/window.h>

#include <stdexcept>
#include <string>

SimpleEngine::Backend::Backend(const std::string &p_windowTitle, uint16_t p_windowWidth, uint16_t p_windowHeight)
    : m_windowTitle(p_windowTitle),
      m_windowWidth(p_windowWidth),
      m_windowHeight(p_windowHeight),
      m_isRunning(false),
      m_elapsedTimeInSeconds(0),
      m_lastUpdateStart(0),
      m_timeSinceTitleUpdate(0)
{
}

const double SimpleEngine::Backend::UpdateElapsedTime() { return m_elapsedTimeInSeconds; }

// Private Functions
void SimpleEngine::Backend::Start()
{
    smtWindowOpen(m_windowWidth, m_windowHeight, m_windowTitle.c_str());
    m_isRunning = true;
}

void SimpleEngine::Backend::Update()
{
    if (!m_isRunning)
    {
        return;
    }

    smtWindowUpdate();
    m_isRunning = smtWindowShouldClose() == SMT_FALSE;
}

void SimpleEngine::Backend::Cleanup() { smtWindowClose(); }