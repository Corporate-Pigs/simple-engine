#pragma once

#include <SDL.h>
#include <SE_inputKey.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace SimpleEngine {

    struct Input {
    friend class Backend;

    public:
        Input();

        void AddKeyBinds(const std::unordered_map<std::string, std::vector<InputKey>>& keyBinds);

        void AddKeyBinds(const std::string& action, const std::vector<InputKey>& keyBinds);

        void RemoveAction(const std::string& action);

        void RemoveKeyBinds(const std::string& action, const std::vector<InputKey>& keyBinds);

        bool IsPerformingAction(const std::string& action);

        void Destroy();
    private:
        void Update(const SDL_Event& event, const double& currentTime);

        std::unordered_map<std::string, std::vector<InputKey>> m_inputMapping;
        std::unordered_map<InputKey, double> m_pressedKeys;
    };
}  // namespace Engine