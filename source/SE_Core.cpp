#include "SE_Core.h"

#include "internal/SE_EngineContext.h"

SimpleEngine::SE_EngineContext k_context;

void SimpleEngine::SE_Init(const std::string& windowTitle, const uint32_t windowWidth, const uint32_t windowHeight) {
    k_context.graphics.num = 69;
}

void SimpleEngine::SE_Destroy() {
    printf("ge %d\n", k_context.graphics.num);
}

void SimpleEngine::SE_Update() {
    k_context.graphics.num++;
}

bool SimpleEngine::SE_IsClosing() {
    return true;
}

uint64_t SimpleEngine::SE_GetTicks() {
    return 0;
}