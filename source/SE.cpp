#include "SE.h"

SimpleEngine::SimpleEngine::SimpleEngine(const std::string& windowTitle, const uint32_t windowWidth,
                                         const uint32_t windowHeight) {}

SimpleEngine::SimpleEngine::~SimpleEngine() {}

void SimpleEngine::SimpleEngine::Run() {
    while (1) {
        SE_Update();
        Update();
        m_graphics.Render();
    }
    Terminate();
}

void SimpleEngine::SimpleEngine::SE_Update() {
    // get window input, etc
}

uint64_t SimpleEngine::SimpleEngine::GetTicks() { return 0; }