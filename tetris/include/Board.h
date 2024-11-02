#pragma once

#include <Window.h>
#include <Constants.h>

#include <vector>
#include <memory>
#include <unordered_map>

class Tetramino;
class Sprite;
class Block;
class StatsManager;
class ParticleSystem;

class Board 
{
public:
    ~Board() = default;

    Board() = delete;
    Board(const Window&);

    auto Draw() -> void;

    [[nodiscard]] auto& GetCurrentPlayingTetramino() { return m_CurrentTetramino; }
    [[nodiscard]] auto& GetGhostTetramino() { return m_GhostTetramino; }
    [[nodiscard]] auto& GetSpriteMap() { return m_SpriteMap; }

    auto& Matrix() { return m_Matrix; }
    const auto& Matrix() const { return m_Matrix; }

    auto& RowsToClear() { return m_RowsToClear; }
    const auto& RowsToClear() const { return m_RowsToClear; }

    auto& StatsManagers() { return m_StatsManager; }
    const auto& StatsManagers() const { return m_StatsManager; }

    auto AddParticleSystem(const std::shared_ptr<ParticleSystem>& ps) { m_DestroyBlockPSs.push_back(ps); }
    auto ClearParticleSystemCache() { m_DestroyBlockPSs.clear(); }

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
    std::unordered_map<uint8_t, std::shared_ptr<Sprite>> m_SpriteMap = {};
    std::vector<std::shared_ptr<Sprite>> m_WallBlocks = {};

    std::shared_ptr<Sprite> m_StatsBgTexture = {};

    std::shared_ptr<StatsManager> m_StatsManager = {};

    std::vector<std::shared_ptr<ParticleSystem>> m_DestroyBlockPSs = {};
};