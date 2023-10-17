#include <fsm/board/StateBlink.h>
#include <Tetramino.h>
#include <Board.h>
#include <Input.h>
#include <Types.h>
#include <Texture.h>

StateBlink::StateBlink(const Window& window, Board& board) : FSM{window}, m_Board{board}
{
    m_Board.RowsToClear().clear();
    m_BlinkTimer = {};
}

auto StateBlink::OnStateEnter() -> void
{
    m_Board.m_IsBlinking = true;
}

auto StateBlink::OnStateUpdate(float deltaTime) -> std::shared_ptr<IState>
{
    const auto sdlRenderer = static_cast<SDL_Renderer*>(m_Window.GetRendererHandle());

    const auto& currentTetramino = m_Board.GetCurrentPlayingTetramino();

    auto& boardMatrix = m_Board.Matrix();

    m_BlinkTimer += deltaTime;

    if (m_BlinkTimer >= MaxBlinkTime)
    {
        OnStateExit();
        return GetState(StateType::Clear);
    }

    for (const auto row : m_Board.RowsToClear())
    {
        for (auto columnIdx = 1ul; columnIdx != (Columns - 1); columnIdx++)
        {
            int index = row * Columns + columnIdx;

            auto& block = m_Board.GetTextureMap()[boardMatrix[index]];
            const auto alphaMod = static_cast<uint8_t>(abs((std::numeric_limits<uint8_t>::max() * sin(m_BlinkTimer) / pow(m_BlinkTimer, 2))));
            
            block->SetPositionOnScreen(columnIdx * BlockSize, row * BlockSize);
            block->Draw(sdlRenderer, alphaMod);
        }
    }

    return shared_from_this();
}

auto StateBlink::OnStateExit() -> void
{
    m_BlinkTimer = {};
    m_Board.m_IsBlinking = false;
}