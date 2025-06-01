#include "SE_smtInput.h"

SimpleEngine::Input::Input() : m_inputMapping(), m_pressedKeys() { m_pressedKeys.reserve(8); }

void SimpleEngine::Input::AddKeyBinds(const std::unordered_map<std::string, std::vector<InputKey>>& keyBinds) {}

void SimpleEngine::Input::AddKeyBinds(const std::string& action, const std::vector<InputKey>& keyBinds) {}

void SimpleEngine::Input::RemoveAction(const std::string& action) {}

void SimpleEngine::Input::RemoveKeyBinds(const std::string& action, const std::vector<InputKey>& keyBinds) {}

bool SimpleEngine::Input::IsPerformingAction(const std::string& action) { return false; }

bool SimpleEngine::Input::IsPressingAnyKey() { return false; }

void SimpleEngine::Input::Update() { }

void SimpleEngine::Input::Destroy() {}