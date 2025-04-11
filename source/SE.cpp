#include "SE.h"

SimpleEngine::SimpleEngine::SimpleEngine(const std::string& windowTitle, const uint32_t windowWidth,
                                         const uint32_t windowHeight) {}

SimpleEngine::SimpleEngine::~SimpleEngine() {}

void SimpleEngine::SimpleEngine::Start() {
    while (1) {
        SE_Update();
        Update();
        SE_Draw();
    }
}

void SimpleEngine::SimpleEngine::Update() {}

void SimpleEngine::SimpleEngine::SE_Update() {
    // get window input, etc
}

void SimpleEngine::SimpleEngine::SE_Draw() {
    // draw sprits, swap buffer
}

void SimpleEngine::SimpleEngine::End() {}

void SimpleEngine::SimpleEngine::DrawSprite() { m_graphics.Initialize(); }

uint64_t SimpleEngine::SimpleEngine::GetTicks() { return 0; }