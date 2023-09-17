#pragma once

#include <Window.h>
#include <Constants.h>

#include <vector>
#include <memory>
#include <unordered_map>

class Tetramino;
class Texture;
class Block;
class IState;
class StatePlay;

class Board 
{
public:
    ~Board() = default;

    Board() = default;
    Board(const Window&);

    auto Update(float deltaTime) -> void;
    auto Draw() -> void;

    auto GetCurrentPlayingTetramino() const -> std::shared_ptr<Tetramino>;

private:
    auto GetRandomTetramino() -> std::shared_ptr<Tetramino>&;

    std::vector<uint8_t> m_OriginalMatrix, m_Matrix = {};

    Window m_Window = {};

private:
    std::shared_ptr<Tetramino> m_CurrentTetramino = {};
    std::array<std::shared_ptr<Tetramino>, MaxNumTetraminos> m_Tetraminos = {};
    std::unordered_map<uint8_t, std::shared_ptr<Texture>> m_TextureMap = {};

    // FSM
    std::shared_ptr<StatePlay> m_StatePlay = {};
    std::shared_ptr<IState> m_CurrentState = {};
};