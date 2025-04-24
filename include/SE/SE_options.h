#pragma once

#include <cinttypes>
#include <string>

namespace SimpleEngine {

struct Options {
    std::string windowTitle;
    uint16_t windowWidth, windowHeight;
};

}  // namespace SimpleEngine