#pragma once

#include <functional>
#include <memory>

#include <Types.h>

#include <glad/glad.h>
#include <SDL2/SDL.h>

struct WindowProps
{
	int width = {};
	int height = {};
};

class Window
{
public:
	Window(const unsigned depth, const unsigned majorVersion, const unsigned minorVersion);
	Window(const Window&) = delete;
	Window(Window&&) = delete;

	auto Update(const std::function<void(float)>& renderHook, const std::function<void(float)>& renderGUIHook) -> void;
	auto CloseWindow() -> void;

	[[nodiscard]] auto Props() const { return m_Props; }
	[[nodiscard]] auto IsOpen() const { return m_IsOpen; }
	[[nodiscard]] auto& IsPaused() { return m_IsPaused; }
	[[nodiscard]] const auto& IsPaused()const { return m_IsPaused; }

	[[nodiscard]] auto GetGLContext() const { return m_SdlGLContext; }
	[[nodiscard]] const auto& GetKeyState() const { return m_Keys; }

	auto SetPostProcessingEnabled(const bool enabled) { m_IsPostProcessingEnabled = enabled; }
	auto SetPostProcessingByType(const PostProcessingType type) { m_CurrentScenePP = m_ScenePPsMap[type]; }
	auto IsPostProcessingEnabled() const { return m_IsPostProcessingEnabled; }

	float m_DeltaTime = {};
private:
	WindowProps m_Props = {};
	SDL_Window* m_SdlWindowHandle = {};
	SDL_GLContext m_SdlGLContext = {};
	std::unordered_map<PostProcessingType, std::shared_ptr<class PostProcessing>> m_ScenePPsMap = {};
	std::shared_ptr<class PostProcessing> m_CurrentScenePP = {};
	std::shared_ptr<class PostProcessing> m_PausePP = {};

	bool m_IsOpen = {};
	bool m_IsPaused = {};
	bool m_IsPostProcessingEnabled = {};
	const unsigned char* m_Keys = {};

private:
	auto RenderSceneWithoutPostProcessing(const std::function<void(float)>& renderHook) -> void;
	auto RenderGui(const std::function<void(float)> &renderGuiHook) -> void;
};