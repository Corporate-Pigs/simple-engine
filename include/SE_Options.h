#pragma once

#include <cinttypes>
#include <string>

namespace SimpleEngine {

struct SE_Options {
    std::string windowTitle;
    uint16_t windowWidth, windowHeight;
};

}  // namespace SimpleEngine