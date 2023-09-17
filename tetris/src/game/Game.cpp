#include <game/Game.h>

#include <Window.h>
#include <Board.h>

Game::Game()
{
    m_Context = std::make_shared<Window>();
    m_Board = std::make_shared<Board>(*m_Context.get());
}

auto Game::Update() -> void
{
    m_Context->Update([&](float deltaTime){
        m_Board->Update(deltaTime);
        m_Board->Draw();
    });
}
