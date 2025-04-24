#include "SE_smtBackend.h"

#include <stdexcept>
#include <string>

SimpleEngine::Backend::Backend(const std::string &p_windowTitle, uint16_t p_windowWidth, uint16_t p_windowHeight)
    : m_windowTitle(p_windowTitle),
      m_windowWidth(p_windowWidth),
      m_windowHeight(p_windowHeight),
      m_isRunning(false),
      m_elapsedTimeInSeconds(0),
      m_lastUpdateStart(0)
{
}

const double SimpleEngine::Backend::GetElapsedTime() { return m_elapsedTimeInSeconds; }

// Private Functions
void SimpleEngine::Backend::Start()
{
   
    m_isRunning = true;
}

void SimpleEngine::Backend::Update()
{
    if (!m_isRunning)
    {
        return;
    }

}

void SimpleEngine::Backend::Cleanup()
{

}