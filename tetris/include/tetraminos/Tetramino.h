#pragma once

#include <vector>
#include <memory>
#include <cmath>

#include <Types.h>
#include <Constants.h>
#include <Color.h>
#include <Window.h>

#pragma region SHAPES

const std::vector<uint8_t> TetraminoIMap = {
    0,0,0,0,
    2,2,2,2,
    0,0,0,0,
    0,0,0,0
};

const std::vector<uint8_t> TetraminoJMap = {
    0,0,0,
    3,3,3,
    0,0,3
};

const std::vector<uint8_t> TetraminoLMap = {
    0,0,0,
    4,4,4,
    4,0,0
};

const std::vector<uint8_t> TetraminoOMap = {
    5,5,
    5,5
};

const std::vector<uint8_t> TetraminoSMap = {
    0,6,6,
    6,6,0,
    0,0,0
};

const std::vector<uint8_t> TetraminoTMap = {
    0,0,0,
    0,7,0,
    7,7,7,
};

const std::vector<uint8_t> TetraminoZMap = {
    8,8,0,
    0,8,8,
    0,0,0
};

#pragma endregion

class Tetramino
{
public:
    ~Tetramino() = default;
    Tetramino(const Window&, const TetraminoType&);

    auto Translate(const DirectionType&) -> void;
    auto Rotate(const std::vector<uint8_t>& matrixBoard, const DirectionType&) -> void;
    auto Reset() -> void;
    auto Draw() -> void;

    [[nodiscard]] const auto& Matrix() const { return m_TetraminoMatrix; }
    [[nodiscard]] auto& Matrix() { return m_TetraminoMatrix; }

    [[nodiscard]] const auto& Blocks() const { return m_Blocks; };
    [[nodiscard]] auto& Blocks() { return m_Blocks; };

    [[nodiscard]] auto GetIndex() const { return m_Index; };
    [[nodiscard]] auto GetColor() const { return m_Color; };
    [[nodiscard]] auto GetMatrixRowLength() const { return static_cast<int>(std::sqrt(m_TetraminoMatrix.size())); };

    [[nodiscard]] auto CanMove(const std::vector<uint8_t>& matrixBoard, const DirectionType direction) -> bool;

    [[nodiscard]] auto GetBlockPositionInMatrix() -> std::vector<uint8_t>;
    [[nodiscard]] auto GetBlockPositionInPixels() -> std::vector<std::tuple<int, int>>;

    int deltaX = {};
    int deltaY = {};

private:
    std::vector<uint8_t> m_TetraminoMatrix, m_OriginalTetraminoMatrix = {};
    std::vector<std::shared_ptr<class Texture>> m_Blocks = {};

    TetraminoType m_Type = {};
    Color m_Color = {};
    Window m_Window = {};

    int m_Index = {};
};