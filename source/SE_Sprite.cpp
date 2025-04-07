#include "SE_Sprite.h"

#include <SDL_image.h>

SimpleEngine::SE_Sprite::SE_Sprite(const std::string& asset) : k_asset(asset) {

}

uint64_t SimpleEngine::SE_GetTicks() {
    return 0;
}