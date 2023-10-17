#include <game/Game.h>

#include <Window.h>
#include <Board.h>

#include <fsm/game/StateSetup.h>
#include <fsm/game/StateMainMenu.h>

#include <fsm/board/StatePlay.h>
#include <fsm/board/StateCheck.h>
#include <fsm/board/StateClear.h>
#include <fsm/board/StateBlink.h>

Game::Game()
{
    m_Context = std::make_shared<Window>();
    m_Board = std::make_shared<Board>(*m_Context.get());

    // FSM (setup)
    m_SetupState = std::make_shared<StateSetup>(*m_Context.get());
    m_MainMenuState = std::make_shared<StateMainMenu>(*m_Context.get(), *this);

    // FSM (board)
    m_StatePlay = std::make_shared<StatePlay>(*m_Context.get(), *m_Board.get());
    m_StateCheck = std::make_shared<StateCheck>(*m_Context.get(), *m_Board.get());
    m_StateBlink = std::make_shared<StateBlink>(*m_Context.get(), *m_Board.get());
    m_StateClear = std::make_shared<StateClear>(*m_Context.get(), *m_Board.get());

    m_SetupState->SetState(StateType::MainMenu, m_MainMenuState);
    m_MainMenuState->SetState(StateType::Setup, m_SetupState);
    m_MainMenuState->SetState(StateType::Play, m_StatePlay);

    m_StatePlay->SetState(StateType::Check, m_StateCheck);
    m_StateCheck->SetState(StateType::Play, m_StatePlay);
    m_StateCheck->SetState(StateType::Blink, m_StateBlink);
    m_StateBlink->SetState(StateType::Clear, m_StateClear);
    m_StateClear->SetState(StateType::Play, m_StatePlay);

    m_CurrentState = m_SetupState;
    m_CurrentState->OnStateEnter();
}

auto Game::Update() -> void
{
    m_Context->Update([&](float deltaTime)
    {
        if (m_CurrentState != m_SetupState && m_CurrentState != m_MainMenuState)
        {
            m_Board->Draw();
        }

        m_CurrentState = m_CurrentState->OnStateUpdate(deltaTime);
    });
}
