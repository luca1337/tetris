#include <Window.h>
#include <GLUtils.h>
#include <ResourceManager.h>
#include <rendering/Shader.h>
#include <Input.h>
#include <rendering/PostProcessing.h>

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <cstdint>
#include <unordered_map>
#include <filesystem>
#include <iostream>

Window::Window(const unsigned depth, const unsigned majorVersion, const unsigned minorVersion) : m_Props({530, 600})
{
    // if (depth == 0) depth = 32;
    // if (majorVersion == 0) majorVersion = 3;
    // if (minorVersion == 0) minorVersion = 3;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, majorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minorVersion);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depth);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("SDL initialization failed: %s", SDL_GetError());
        CloseWindow();
        return;
    }

    m_SdlWindowHandle = SDL_CreateWindow("Tetris (C++)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Props.width, m_Props.height, SDL_WINDOW_OPENGL);
    if (!m_SdlWindowHandle)
    {
        SDL_Log("SDL window creation failed: %s", SDL_GetError());
        CloseWindow();
        return;
    }

    m_SdlGLContext = SDL_GL_CreateContext(m_SdlWindowHandle);
    if (!m_SdlGLContext)
    {
        SDL_Log("OpenGL context creation failed: %s", SDL_GetError());
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

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) == -1) 
    {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        return;
    }

    SDL_GL_SetSwapInterval(0);

    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_Keys = const_cast<uint8_t *>(SDL_GetKeyboardState(nullptr));

    m_IsOpen = true;

    // init Post-Processing 

    m_ScenePPsMap = std::unordered_map<PostProcessingType, std::shared_ptr<PostProcessing>>{
        {PostProcessingType::cctv, std::make_shared<PPCCTV>(*this) },
        {PostProcessingType::chroma, std::make_shared<PPChromaticAberration>(*this) },
        {PostProcessingType::edge_detection, std::make_shared<PPEdgeDetection>(*this) },
        {PostProcessingType::embossing, std::make_shared<PPEmbossing>(*this) },
    };

    m_CurrentScenePP = m_ScenePPsMap[PostProcessingType::chroma];
    
    m_PausePP = std::make_shared<PPBlur>(*this);

    m_IsPostProcessingEnabled = true;
}

auto Window::Update(const std::function<void(float)> &renderHook, const std::function<void(float)> &renderGUIHook) -> void
{
    auto prevTime = SDL_GetPerformanceCounter();

    while (m_IsOpen)
    {
        auto ev = SDL_Event{};
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
            {
                CloseWindow();
            }
        }

        const auto currentTime = SDL_GetPerformanceCounter();
        m_DeltaTime = static_cast<float>(currentTime - prevTime) / SDL_GetPerformanceFrequency();
        prevTime = currentTime;

        if (m_IsPaused) { m_DeltaTime = {}; }

        if (m_IsPaused)
        {
            m_PausePP->RenderToScene(m_DeltaTime, [&](){ RenderSceneWithoutPostProcessing(renderHook); }, [&](){ RenderGui(renderGUIHook); });
        }
        else
        {
            if (m_IsPostProcessingEnabled)
            {
                m_CurrentScenePP->RenderToScene(m_DeltaTime, [&](){ RenderSceneWithoutPostProcessing(renderHook); }, [&](){ RenderGui(renderGUIHook); });
            }
            else
            {
                RenderSceneWithoutPostProcessing(renderHook);
                RenderGui(renderGUIHook);
            }
        }

        SDL_GL_SwapWindow(m_SdlWindowHandle);
    }
}

auto Window::CloseWindow() -> void
{
    if (m_SdlWindowHandle)
    {
        SDL_DestroyWindow(m_SdlWindowHandle);
        m_SdlWindowHandle = nullptr;
    }

    TTF_Quit();
    SDL_Quit();
    m_IsOpen = false;
}

auto Window::RenderSceneWithoutPostProcessing(const std::function<void(float)> &renderHook) -> void
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.13f, 0.13f, 0.13f, 1.0f);

    if (renderHook != nullptr)
    {
        renderHook(m_DeltaTime);
    }
}

auto Window::RenderGui(const std::function<void(float)> &renderGuiHook) -> void
{
    if (renderGuiHook != nullptr)
    {
        renderGuiHook(m_DeltaTime);
    }
}