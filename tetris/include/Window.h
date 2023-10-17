#pragma once

#include <functional>

struct WindowProps
{
	int width = {};
	int height = {};
};

struct SDL_Window;
struct SDL_Renderer;

class Window
{
public:
	Window();
	Window(const Window&) = delete;
	Window(Window&&) = delete;

	auto Update(const std::function<void(float)>& renderHook) -> void;
	auto CloseWindow() -> void;

	[[nodiscard]] auto Props() const { return m_Props; }
	[[nodiscard]] auto IsOpen() const { return m_IsOpen; }

	[[nodiscard]] auto GetRendererHandle() const { return m_SdlRendererHandle; }
	[[nodiscard]] const auto& GetKeyState() const { return m_Keys; }

	float m_DeltaTime = {};
private:
	WindowProps m_Props = {};
	SDL_Window* m_SdlWindowHandle = {};
	SDL_Renderer* m_SdlRendererHandle = {};

	bool m_IsOpen = {};
	const unsigned char* m_Keys = {};
};