#include <Window.h>

#include <cstdint>
#include <unordered_map>
#include <filesystem>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL2/SDL_ttf.h>

Window::Window() : m_Props({ 530, 600 })
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_Log("SDL initialization failed: %s", SDL_GetError());
		CloseWindow();
		return;
	}

	m_SdlWindowHandle = SDL_CreateWindow("Tetris (C++)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Props.width, m_Props.height, 0x0);
	if (!m_SdlWindowHandle)
	{
		SDL_Log("SDL window creation failed: %s", SDL_GetError());
		CloseWindow();
		return;
	}

	m_SdlRendererHandle = SDL_CreateRenderer(m_SdlWindowHandle, -1, SDL_RENDERER_ACCELERATED);
	if (!m_SdlRendererHandle)
	{
		SDL_Log("SDL renderer creation failed: %s", SDL_GetError());
		CloseWindow();
		return;
	}

	if (TTF_Init() < 0)
	{
		SDL_Log("TTF initialization failed: %s", TTF_GetError());
		CloseWindow();
		return;
	}

	auto windowIcon = IMG_Load("../../tetris/resources/tetris.png");
	if (!windowIcon)
	{
		SDL_Log("Couldn't load window icon: %s", SDL_GetError());
	}
	else
	{
		SDL_SetWindowIcon(m_SdlWindowHandle, windowIcon);
		SDL_FreeSurface(windowIcon);
	}

	m_Keys = const_cast<uint8_t*>(SDL_GetKeyboardState(nullptr));

	m_IsOpen = true;
}

auto Window::Update(const std::function<void(float)>& renderHook) -> void
{
	auto deltaTime = 0.0;
	auto prevTime = SDL_GetPerformanceCounter();

	while (m_IsOpen)
	{
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT)
			{
				CloseWindow();
			}
		}

		// Calcola delta time
		const auto currentTime = SDL_GetPerformanceCounter();
		deltaTime = static_cast<double>(currentTime - prevTime) / SDL_GetPerformanceFrequency();
		prevTime = currentTime;

		// clear screen and add alpha blending
		SDL_SetRenderDrawBlendMode(m_SdlRendererHandle, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(m_SdlRendererHandle, 25, 25, 25, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(m_SdlRendererHandle);

		if (renderHook != nullptr)
		{
			renderHook(deltaTime);
		}

		SDL_RenderPresent(m_SdlRendererHandle);
	}
}

auto Window::CloseWindow() -> void
{
	if (m_SdlRendererHandle)
	{
		SDL_DestroyRenderer(m_SdlRendererHandle);
		m_SdlRendererHandle = nullptr;
	}

	if (m_SdlWindowHandle)
	{
		SDL_DestroyWindow(m_SdlWindowHandle);
		m_SdlWindowHandle = nullptr;
	}

	TTF_Quit();
	SDL_Quit();
	m_IsOpen = false;
}
