#pragma once

#include <Window.h>
#include <Constants.h>

#include <vector>
#include <memory>
#include <unordered_map>

class Tetramino;
class Texture;
class Block;

class Board 
{
public:
    ~Board() = default;

    Board() = default;
    Board(const Window&);

    auto Draw() -> void;

    [[nodiscard]] auto& GetCurrentPlayingTetramino() { return m_CurrentTetramino; }
    [[nodiscard]] auto& GetGhostTetramino() { return m_GhostTetramino; }
    [[nodiscard]] auto GetTextureMap() const { return m_TextureMap; }

    auto& Matrix() { return m_Matrix; }
    const auto& Matrix() const { return m_Matrix; }

    auto& RowsToClear() { return m_RowsToClear; }
    const auto& RowsToClear() const { return m_RowsToClear; }

    auto RandomizeCurrentPlayingTetramino() -> void;

    bool m_IsBlinking = {};

private:
    const Window& m_Window;

private:
    std::vector<uint8_t> m_OriginalMatrix, m_Matrix, m_RowsToClear = {};
    std::shared_ptr<Tetramino> m_CurrentTetramino = {};
    std::shared_ptr<Tetramino> m_NextTetramino = {};
    std::shared_ptr<Tetramino> m_GhostTetramino = {};
    std::vector<std::shared_ptr<Tetramino>> m_Tetraminos = {};
    std::unordered_map<uint8_t, std::shared_ptr<Texture>> m_TextureMap = {};
};