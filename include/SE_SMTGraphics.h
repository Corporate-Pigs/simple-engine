#pragma once

#include <iostream>

class SE_Graphics {
   public:
    static constexpr const char* Name() { return "SMT"; }

    void Initialize() {
        std::cout << "Initializing SMT..." << std::endl;
        // SMT initialization code
    }

    void Render() {
        std::cout << "Rendering with SMT..." << std::endl;
        // SMT rendering code
    }

    void Shutdown() {
        std::cout << "Shutting down SMT..." << std::endl;
        // SMT shutdown code
    }
};
