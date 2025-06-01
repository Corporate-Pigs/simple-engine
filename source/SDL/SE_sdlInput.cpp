#include "SE_sdlInput.h"

#include <algorithm>

SimpleEngine::InputKey GetSimpleEngineKeyFromKeyboard(const SDL_Event& event)
{
    auto key = (int32_t)event.key.keysym.sym;

    // ASCII, direct mapping. CAPSLOCK is the first special character
    if (key < (int32_t)SDLK_CAPSLOCK)
    {
        return (SimpleEngine::InputKey)key;
    }

    // Special keys
    if (key < (int32_t)SDLK_NUMLOCKCLEAR)
    {
        uint8_t acc = 0;

        // There's a jump gap between SDLK_PAGEUP and SDLK_END
        if (key > (int32_t)SDLK_PAGEUP)
        {
            acc = 1;
        }

        return (SimpleEngine::InputKey)(
            (key ^ SDLK_SCANCODE_MASK) +
            (SimpleEngine::InputKey::KB_CAPSLOCK - (SDLK_CAPSLOCK ^ SDLK_SCANCODE_MASK)) - acc);
    }

    // Modifier keys
    return (SimpleEngine::InputKey)((key ^ SDLK_SCANCODE_MASK) +
                                    (SimpleEngine::InputKey::KB_LCTRL - (SDLK_LCTRL ^ SDLK_SCANCODE_MASK)));
}

SimpleEngine::InputKey GetSimpleEngineKeyFromMouse(const SDL_Event& event)
{
    auto key = (int32_t)event.button.button;

    return (SimpleEngine::InputKey)(key + SimpleEngine::InputKey::M_LEFT - 1);
}

SimpleEngine::Input::Input() : m_inputMapping(), m_pressedKeys() 
{
    m_pressedKeys.reserve(8); 
}

void SimpleEngine::Input::AddKeyBinds(const std::unordered_map<std::string, std::vector<InputKey>>& keyBinds)
{
    for (auto& kv : keyBinds)
    {
        AddKeyBinds(kv.first, kv.second);
    }
}

void SimpleEngine::Input::AddKeyBinds(const std::string& action, const std::vector<InputKey>& keyBinds)
{
    if (m_inputMapping.find(action) == m_inputMapping.end())
    {
        m_inputMapping.emplace(action, keyBinds);
        return;
    }

    std::vector<InputKey>* actionKeys = &m_inputMapping.at(action);

    for (InputKey key : keyBinds)
    {
        // vector already contains key
        if (std::find(actionKeys->begin(), actionKeys->end(), key) != actionKeys->end())
        {
            continue;
        }

        actionKeys->emplace_back(key);
    }
}

void SimpleEngine::Input::RemoveAction(const std::string& action) { m_inputMapping.erase(action); }

void SimpleEngine::Input::RemoveKeyBinds(const std::string& action, const std::vector<InputKey>& keyBinds)
{
    if (m_inputMapping.find(action) == m_inputMapping.end())
    {
        return;
    }

    std::vector<InputKey>* actionKeys = &m_inputMapping.at(action);

    for (InputKey key : keyBinds)
    {
        // vector doesn't contain this key
        auto iter = std::find(actionKeys->begin(), actionKeys->end(), key);
        if (iter == actionKeys->end())
        {
            continue;
        }

        actionKeys->erase(iter);
    }

    if (actionKeys->empty())
    {
        m_inputMapping.erase(action);
    }
}

bool SimpleEngine::Input::IsPerformingAction(const std::string& action)
{
    if (m_inputMapping.find(action) == m_inputMapping.end()) return false;

    std::vector<InputKey>* keys = &m_inputMapping.at(action);

    for (InputKey key : *keys)
    {
        if (m_pressedKeys.find(key) != m_pressedKeys.end())
        {
            return true;
        }
    }

    return false;
}

bool SimpleEngine::Input::IsPressingAnyKey()
{
    return !m_pressedKeys.empty();
}

void SimpleEngine::Input::Update(const SDL_Event& event, const double& currentTime)
{
    SimpleEngine::InputKey simpleEngineKey;

    switch (event.type)
    {
        case SDL_KEYDOWN:
            simpleEngineKey = GetSimpleEngineKeyFromKeyboard(event);

            if (m_pressedKeys.find(simpleEngineKey) == m_pressedKeys.end())
            {
                m_pressedKeys[simpleEngineKey] = currentTime;
            }

            break;
        case SDL_KEYUP:
            simpleEngineKey = GetSimpleEngineKeyFromKeyboard(event);

            m_pressedKeys.erase(simpleEngineKey);
            break;

        case SDL_MOUSEBUTTONDOWN:
            simpleEngineKey = GetSimpleEngineKeyFromMouse(event);

            // Todo: Add more info than currentTime
            if (m_pressedKeys.find(simpleEngineKey) == m_pressedKeys.end())
            {
                m_pressedKeys[simpleEngineKey] = currentTime;
            }

            break;

        case SDL_MOUSEBUTTONUP:
            simpleEngineKey = GetSimpleEngineKeyFromMouse(event);

            m_pressedKeys.erase(simpleEngineKey);
            break;

            /*case SDL_MOUSEMOTION:
            case SDL_MOUSEWHEEL:*/
    }

    return;
}

void SimpleEngine::Input::Destroy()
{
    m_inputMapping.clear();
    m_pressedKeys.clear();
}