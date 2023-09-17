#pragma once

#include <functional>

struct WindowProps
{
	int width = {};
	int height = {};
};

class Window
{
public:
	Window();

	auto Update(const std::function<void(float)>& renderHook) -> void;

	[[nodiscard]] auto Props() const { return m_Props; }
	[[nodiscard]] auto IsOpen() const { return m_IsOpen; }

	[[nodiscard]] auto GetRendererHandle() const { return m_SdlRendererHandle; }
	[[nodiscard]] const auto& GetKeyState() const { return m_Keys; }

	float m_DeltaTime = {};
private:
	WindowProps m_Props = {};
	void* m_SdlWindowHandle = {};
	void* m_SdlRendererHandle = {};

	bool m_IsOpen = {};
	const unsigned char* m_Keys = {};
};