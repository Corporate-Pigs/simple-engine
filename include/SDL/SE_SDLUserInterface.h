#pragma once

namespace SimpleEngine {

class SE_UserInterface {
    friend class SimpleEngine;

   private:
    void Initialize();
    void Update();
    void Cleanup();
};

}  // namespace SimpleEngine
