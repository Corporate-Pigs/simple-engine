#pragma once

#include <iostream>

class SE_Graphics {
   public:
    static constexpr const char* Name() { return "SDL"; }

    void Initialize() {
        std::cout << "Initializing SDL..." << std::endl;
        // SDL initialization code
    }

    void Render() {
        std::cout << "Rendering with SDL..." << std::endl;
        // SDL rendering code
    }

    void Shutdown() {
        std::cout << "Shutting down SDL..." << std::endl;
        // SDL shutdown code
    }
};
