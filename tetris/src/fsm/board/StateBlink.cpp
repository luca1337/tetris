#include <fsm/board/StateBlink.h>
#include <Tetramino.h>
#include <Board.h>
#include <Input.h>
#include <StatsManager.h>
#include <Types.h>
#include <Sprite.h>
#include <rendering/Shader.h>
#include <ResourceManager.h>
#include <AudioTrack.h>
#include <rendering/ParticleSystem.h>
#include <Random.h>
#include <Constants.h>

#include <iostream>

StateBlink::StateBlink(const Window &window, Board &board) : FSM{window}, m_Board{board}
{
    m_Board.RowsToClear().clear();
    m_BlinkTimer = {};
}

auto StateBlink::OnStateEnter() -> void
{
    m_Board.m_IsBlinking = true;

    const auto clearFx = ResourceManager::GetFromCache<AudioTrack>({ResourceType::Audio, "Clear"});
    if (clearFx.has_value())
    {
        const auto clearFxValue = clearFx.value();

        clearFxValue->Play(); 
    }
}

float minX, maxX, minY, maxY;

auto StateBlink::OnStateUpdate(float deltaTime) -> std::shared_ptr<IState>
{
    const auto spriteShader = ResourceManager::GetFromCache<Shader>({ResourceType::Shader, "SpriteShader"});

    auto &boardMatrix = m_Board.Matrix();

    m_BlinkTimer += deltaTime;

    if (m_BlinkTimer >= MaxBlinkTime)
    {
        OnStateExit();
        return GetState(StateType::Clear);
    }

    const auto rowsToClear = m_Board.RowsToClear();
    const auto numOfRowsToClear = rowsToClear.size();

    for (auto i = 0ul; i != numOfRowsToClear; ++i)
    {
        auto row = rowsToClear[i];

        for (auto columnIdx = 1ul; columnIdx != (Columns - 1); columnIdx++)
        {
            int index = row * Columns + columnIdx;

            auto spriteMap = m_Board.GetSpriteMap();
            auto it = spriteMap.find(index);

            const float k = 2.0f;
            const float initialFrequency = 10.0f;

            if (it != spriteMap.end() && it->second)
            {
                auto &block = it->second;
                const auto frequency = initialFrequency * std::exp(k * m_BlinkTimer);
                const auto alphaMod = (1.0f / std::exp(k * m_BlinkTimer)) * std::abs(std::sin(frequency * m_BlinkTimer));

                float blockX = columnIdx * BlockSize;
                block->SetAlpha(alphaMod);
                block->Render(spriteShader.value());
            }
        }
    }

    return shared_from_this();
}

auto StateBlink::OnStateExit() -> void
{
    m_BlinkTimer = {};
    m_Board.m_IsBlinking = {};
}