#pragma once

#include <memory>

class Window;
class Board;

/// @brief Questa classe e' la radice del gioco, gestisce tutti gli oggetti in scena ed i loro stati (rendering, eventi, turni, macchine a stati finiti... etc)
class Game
{
public:
    ~Game() = default;
    Game();

    auto Update() -> void;

private:
    std::shared_ptr<Window> m_Context = {};
    std::shared_ptr<Board> m_Board = {};
};