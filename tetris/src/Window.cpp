#include <Window.h>

#include <cstdint>
#include <unordered_map>
#include <SDL2/SDL.h>

Window::Window() : m_Props({ 500, 600 })
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		// handle error
		return;
	}

	m_SdlWindowHandle = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Props.width, m_Props.height, 0x0);
	if (!m_SdlWindowHandle)
	{
		// handle error
		return;
	}

	m_SdlRendererHandle = SDL_CreateRenderer(static_cast<SDL_Window*>(m_SdlWindowHandle), -1, SDL_RENDERER_ACCELERATED);
	if (!m_SdlRendererHandle)
	{
		// handle error
		return;
	}

	m_Keys = const_cast<uint8_t*>(SDL_GetKeyboardState(nullptr));

	m_IsOpen = true;
}

auto Window::Update(const std::function<void(float)>& renderHook) -> void
{
	auto deltaTime = 0.0;
	auto prevTime = SDL_GetPerformanceCounter();

	float timer = 0.0f;

	while (m_IsOpen)
	{
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT)
			{
				m_IsOpen = false;
			}
		}

		// Calcola delta time
		Uint64 currentTime = SDL_GetPerformanceCounter();
		deltaTime = static_cast<double>(currentTime - prevTime) / SDL_GetPerformanceFrequency();
		prevTime = currentTime;

		// clear screen and add alpha blending
		SDL_SetRenderDrawBlendMode(static_cast<SDL_Renderer*>(m_SdlRendererHandle), SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(static_cast<SDL_Renderer*>(m_SdlRendererHandle), 25, 25, 25, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(static_cast<SDL_Renderer*>(m_SdlRendererHandle));

		if (renderHook != nullptr)
		{
			renderHook(deltaTime);
		}

		SDL_RenderPresent(static_cast<SDL_Renderer*>(m_SdlRendererHandle));
	}
}