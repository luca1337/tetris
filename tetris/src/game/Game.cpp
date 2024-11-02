#include <game/Game.h>

#include <Window.h>
#include <Board.h>

#include <gui/Menu.h>

#include <fsm/game/StateSetup.h>

#include <fsm/board/StatePlay.h>
#include <fsm/board/StateCheck.h>
#include <fsm/board/StateClear.h>
#include <fsm/board/StateBlink.h>

#include <Logger.h>
#include <Input.h>
#include <AudioTrack.h>
#include <ResourceManager.h>

auto Game::GetInstance() -> Game &
{
    static auto game = Game{};
    return game;
}

auto Game::Update() -> void
{
    const auto mainTheme = ResourceManager::GetFromCache<AudioTrack>({ResourceType::Audio, "MainMenu"});
    const auto pauseFx = ResourceManager::GetFromCache<AudioTrack>({ResourceType::Audio, "Pause"});

    m_Context->Update([&](float deltaTime)
    {
        if (Input::IsKeyPressed(*m_Context, SDLK_ESCAPE) && m_CurrentState != m_SetupState) 
        {
            m_Context->IsPaused() = !m_Context->IsPaused(); 
            m_Menu->Show(m_Context->IsPaused());

            if (mainTheme.has_value() && pauseFx.has_value())
            {
                if (m_Context->IsPaused())
                {
                    pauseFx.value()->Play();
                    mainTheme.value()->Pause(); 
                }
                else 
                {
                    mainTheme.value()->Resume(); 
                }
            }
        }

        if (m_CurrentState != m_SetupState)
        {
            m_Board->Draw();
        }

        if (!m_Context->IsPaused())
        {
            m_CurrentState = m_CurrentState->OnStateUpdate(deltaTime);
        }
    },
    [&](float deltaTime)
    {
        if (m_Context->IsPaused())
        {
            m_Menu->Render();
        }
    });
}

auto Game::Init() -> void
{
    Logger::Init();

    m_pImpl = new GameImpl{};
    m_pImpl->m_MainCamera = Camera(0, 530, 600, 0);

    m_Context = std::make_shared<Window>(32, 4, 6);
    m_Board = std::make_shared<Board>(*m_Context);

    m_Menu = std::make_shared<Menu>(*m_Context);

    const auto mainMenuTheme = std::make_shared<AudioTrack>("../../tetris/resources/audio/Title.flac");
    const auto playTheme = std::make_shared<AudioTrack>("../../tetris/resources/audio/Play.ogg");
    const auto PauseFx = std::make_shared<AudioTrack>("../../tetris/resources/audio/Pause.flac");
    const auto stageClearFx = std::make_shared<AudioTrack>("../../tetris/resources/audio/StageClear.flac");

    ResourceManager::RegisterResource(ResourceParams{ResourceType::Audio, "MainMenu", mainMenuTheme});
    ResourceManager::RegisterResource(ResourceParams{ResourceType::Audio, "Play", playTheme});
    ResourceManager::RegisterResource(ResourceParams{ResourceType::Audio, "Pause", PauseFx});
    ResourceManager::RegisterResource(ResourceParams{ResourceType::Audio, "Clear", stageClearFx});

    // FSM (board)
    m_StatePlay = std::make_shared<StatePlay>(*m_Context, *m_Board);
    m_StateCheck = std::make_shared<StateCheck>(*m_Context, *m_Board);
    m_StateBlink = std::make_shared<StateBlink>(*m_Context, *m_Board);
    m_StateClear = std::make_shared<StateClear>(*m_Context, *m_Board);

    // FSM (setup)
    m_SetupState = std::make_shared<StateSetup>(*m_Context);
    m_SetupState->SetState(StateType::Play, m_StatePlay);

    m_StatePlay->SetState(StateType::Check, m_StateCheck);
    m_StateCheck->SetState(StateType::Play, m_StatePlay);
    m_StateCheck->SetState(StateType::Blink, m_StateBlink);
    m_StateBlink->SetState(StateType::Clear, m_StateClear);
    m_StateClear->SetState(StateType::Play, m_StatePlay);

    m_CurrentState = m_StatePlay;
    m_CurrentState->OnStateEnter();
}