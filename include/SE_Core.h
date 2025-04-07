#pragma once

#include <cinttypes>
#include <string>

namespace SimpleEngine {

	void SE_Init(const std::string& windowTitle, const uint32_t windowWidth, const uint32_t windowHeight);

	void SE_Destroy();

	void SE_Update();

	bool SE_IsClosing();

	uint64_t SE_GetTicks();

}  // namespace SimpleEngine