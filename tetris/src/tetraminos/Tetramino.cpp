#include <tetraminos/Tetramino.h>
#include <Texture.h>
#include <Utils.h>

#include <SDL2/SDL.h>

struct ivec2
{
    int x = {};
    int y = {};
};

auto IterateMatrix(size_t length, const std::function<void(int, int)>& func)
{
    for (auto i = 0ul; i != length; ++i)
    {
        for (auto j = 0ul; j != length; ++j)
        {
            func(i, j);
        }
    }
}

auto UpdateBlockPosition(const Tetramino& tetramino, int i, int j, int& blockIndex)
{
    const auto tetraminoMatrixIndex = tetramino.Matrix()[j * tetramino.GetMatrixRowLength() + i];

    if (tetraminoMatrixIndex == tetramino.GetIndex())
    {
        const auto& block = tetramino.Blocks()[blockIndex++];
        block->SetPositionOnScreen(tetramino.deltaX + i * BlockSize + (3 * BlockSize), tetramino.deltaY + j * BlockSize);
    }
}

auto BuildTetraminoFromMatrix(SDL_Renderer* sdlRenderer, Tetramino& tetramino, const uint8_t* matrix)
{
    auto blocks = std::vector<std::shared_ptr<Texture>>{};

    std::copy(matrix, matrix + tetramino.Matrix().size(), tetramino.Matrix().begin());

    const auto matrixRowLength = tetramino.GetMatrixRowLength();

    IterateMatrix(matrixRowLength, [&](int i, int j){
        const auto tetraminoMatrixIndex = tetramino.Matrix()[j * matrixRowLength + i];

            if (tetraminoMatrixIndex == tetramino.GetIndex())
            {
                auto blockTexture = std::make_shared<Texture>(sdlRenderer, BlockSize, BlockSize, tetramino.GetColor());
                blockTexture->SetPositionOnScreen(i * BlockSize + (3 * BlockSize), j * BlockSize);
                blocks.push_back(blockTexture);
            }
    });

    return blocks;
}

auto RefreshTetramino(const Tetramino& tetramino)
{
    int blockIndex = 0;

    const auto matrixRowLength = tetramino.GetMatrixRowLength();

    IterateMatrix(matrixRowLength, [&](int i, int j){
        UpdateBlockPosition(tetramino, i, j, blockIndex);
    });
}

Tetramino::Tetramino(const Window& window, const TetraminoType& type) : m_Window{window}, m_Type{type}
{
    auto sdlRenderer = static_cast<SDL_Renderer*>(m_Window.GetRendererHandle());

    switch (type)
    {
    case TetraminoType::I:
        m_Color = Cyan;
        m_Index = TetraminoIIndex;
        m_TetraminoMatrix = std::vector<uint8_t>(TetraminoIMap.size());
        m_OriginalTetraminoMatrix = TetraminoIMap;
        m_Blocks = BuildTetraminoFromMatrix(sdlRenderer, *this, TetraminoIMap.data());
        break;
    
    case TetraminoType::J:
        m_Color = Blue;
        m_Index = TetraminoJIndex;
        m_TetraminoMatrix = std::vector<uint8_t>(TetraminoJMap.size());
        m_OriginalTetraminoMatrix = TetraminoJMap;
        m_Blocks = BuildTetraminoFromMatrix(sdlRenderer, *this, TetraminoJMap.data());
        break;

    case TetraminoType::L:
        m_Color = Orange;
        m_Index = TetraminoLIndex;
        m_TetraminoMatrix = std::vector<uint8_t>(TetraminoLMap.size());
        m_OriginalTetraminoMatrix = TetraminoLMap;
        m_Blocks = BuildTetraminoFromMatrix(sdlRenderer, *this, TetraminoLMap.data());
        break;

    case TetraminoType::O:
        m_Color = Yellow;
        m_Index = TetraminoOIndex;
        m_TetraminoMatrix = std::vector<uint8_t>(TetraminoOMap.size());
        m_OriginalTetraminoMatrix = TetraminoOMap;
        m_Blocks = BuildTetraminoFromMatrix(sdlRenderer, *this, TetraminoOMap.data());
        break;

    case TetraminoType::S:
        m_Color = Green;
        m_Index = TetraminoSIndex;
        m_TetraminoMatrix = std::vector<uint8_t>(TetraminoSMap.size());
        m_OriginalTetraminoMatrix = TetraminoSMap;
        m_Blocks = BuildTetraminoFromMatrix(sdlRenderer, *this, TetraminoSMap.data());
        break;

    case TetraminoType::T:
        m_Color = Magenta;
        m_Index = TetraminoTIndex;
        m_TetraminoMatrix = std::vector<uint8_t>(TetraminoTMap.size());
        m_OriginalTetraminoMatrix = TetraminoTMap;
        m_Blocks = BuildTetraminoFromMatrix(sdlRenderer, *this, TetraminoTMap.data());
        break;

    case TetraminoType::Z:
        m_Color = Red;
        m_Index = TetraminoZIndex;
        m_TetraminoMatrix = std::vector<uint8_t>(TetraminoZMap.size());
        m_OriginalTetraminoMatrix = TetraminoZMap;
        m_Blocks = BuildTetraminoFromMatrix(sdlRenderer, *this, TetraminoZMap.data());
        break;

    default:
        break;
    }
}

auto Tetramino::Translate(const DirectionType& direction) -> void
{
    int deltaXChange = 0;
    int deltaYChange = 0;

    switch (direction)
    {
        case DirectionType::Left:
            deltaXChange = -BlockSize;
            break;
        case DirectionType::Right:
            deltaXChange = BlockSize;
            break;
        case DirectionType::Down:
            deltaYChange = BlockSize;
            break;
        default:
            break;
    }

    deltaX += deltaXChange;
    deltaY += deltaYChange;

    for (auto& block : m_Blocks)
    {
        auto [screenX, screenY] = block->GetPositionOnScreen();
        block->SetPositionOnScreen(screenX + deltaXChange, screenY + deltaYChange);
    }
}

auto Tetramino::Rotate(const std::vector<uint8_t>& matrixBoard, const DirectionType& directionType) -> void
{
    auto canRotate = true;

    auto rotatedTetramino = std::vector<uint8_t>();

    switch (directionType)
    {
    case DirectionType::Right: 
    {
        // ruoto il tetramino e mi faccio tornare la matrice ruotata, poi devo controllare se effettivamente posso ruotarla, quindi simulo una rotazione
        // dopodichè con la matrice ruotata controllo se nella mappa a quegli indici ci sono gia' altri indici per cui la matrice andrebbe in collisione
        // se e' vera la condizione allora ruoto di nuovo, itero il processo finche non trovo un caso che sia vero, qual'ora dovesse tornare false per tutti i casi
        // non posso ruotare il tetramino (SRS System).

        // attualmente il tetramino non viene mai ruotato, se la rotazione e' bloccata per la presenza di altri tetramini semplicemente l'utente non puo' piu' fare nulla

        const auto tempRotatedMatrix = utils::RotateMatrix(m_TetraminoMatrix);

        auto newPositionsToCheck = std::vector<ivec2>{};

        const auto rowLength = static_cast<int>(std::sqrt(tempRotatedMatrix.size()));

        IterateMatrix(rowLength, [&](int i, int j){
            const auto tetraminoMatrixIndex = tempRotatedMatrix[j * rowLength + i];

            if (tetraminoMatrixIndex == GetIndex())
            {
                newPositionsToCheck.push_back(ivec2{deltaX + i * BlockSize + (3 * BlockSize), deltaY + j * BlockSize});
            }
        });

        for(const auto pos: newPositionsToCheck)
        {
            const auto x = pos.x / BlockSize;
            const auto y = pos.y / BlockSize;
            const auto matrixIndex = y * Columns + x;

            if ( (matrixBoard[matrixIndex] >= 1 && matrixBoard[matrixIndex] <= 8) || matrixIndex > matrixBoard.size() )
            {
                canRotate = false;
                break;
            }
        }

        rotatedTetramino = tempRotatedMatrix;
    } 
    break;
    default: break;
    }

    if (canRotate)
    {
        m_TetraminoMatrix = rotatedTetramino;
        RefreshTetramino(*this);
    }
}

auto Tetramino::Reset() -> void
{
    m_TetraminoMatrix = m_OriginalTetraminoMatrix;

    int blockIndex = 0;

    const auto matrixRowLength = GetMatrixRowLength();

    IterateMatrix(matrixRowLength, [&](int i, int j){
        UpdateBlockPosition(*this, i, j, blockIndex);
    });

    deltaX = 0;
    deltaY = 0;
}

auto Tetramino::Draw() -> void
{
    auto sdlRenderer = static_cast<SDL_Renderer*>(m_Window.GetRendererHandle());

    for(const auto& block: m_Blocks)
    {
        block->Draw(sdlRenderer, 0xFF);
    }
}

auto Tetramino::CanMove(const std::vector<uint8_t>& matrixBoard, const DirectionType direction) -> bool
{
    auto canMove = true;

    for(const auto index: GetBlockPositionInMatrix())
    {
        const auto potentialIndex = (direction == DirectionType::Right) ? index + 1 :
                              (direction == DirectionType::Left) ? index - 1 :
                              (direction == DirectionType::Down) ? index + Columns : index;

        const auto matrixValueAtIndex = matrixBoard[potentialIndex];

        constexpr auto minCollisionIndex = 1;
        constexpr auto maxCollisionIndex = 8;

        if ( (matrixValueAtIndex >= minCollisionIndex && matrixValueAtIndex <= maxCollisionIndex) || potentialIndex > matrixBoard.size())
        {
            canMove = false;
            break;
        }
    }

    return canMove;
}

auto Tetramino::GetBlockPositionInMatrix() -> std::vector<uint8_t>
{
    auto matrixPositionsIndexes = std::vector<uint8_t>();

    for(const auto& block: m_Blocks)
    {
        const auto [x, y] = block->GetPositionOnScreen();
        
        const auto matrixX = x / BlockSize;
        const auto matrixY = y / BlockSize;

        const auto matrixIndex = matrixY * Columns + matrixX;

        matrixPositionsIndexes.push_back(matrixIndex);
    }

    return matrixPositionsIndexes;
}

auto Tetramino::GetBlockPositionInPixels() -> std::vector<std::tuple<int, int>>
{
    auto positions = std::vector<std::tuple<int, int>>{};

    for(const auto& block: m_Blocks)
    {
        positions.push_back(block->GetPositionOnScreen());
    }

    return positions;
}