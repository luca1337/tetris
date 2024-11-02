#include <fsm/game/StateSetup.h>

#include <Input.h>
#include <Utils.h>
#include <RenderText.h>
#include <ResourceManager.h>
#include <rendering/Shader.h>
#include <AudioTrack.h>

StateSetup::StateSetup(const Window &window) : FSM{window}
{
    m_NintendoText = std::make_shared<RenderText>("../../tetris/resources/fonts/nintendo.ttf", 64);
    if (!m_NintendoText)
    {
        SDL_Log("Couldn't create text m_NintendoText");
        return;
    }

    m_PressToSkipText = std::make_shared<RenderText>("../../tetris/resources/fonts/nintendo.ttf", 16);
    if (!m_PressToSkipText)
    {
        SDL_Log("Couldn't create text m_PressToSkipText");
        return;
    }

    m_TetrisText = std::make_shared<RenderText>("../../tetris/resources/fonts/nintendo.ttf", 16);
    if (!m_TetrisText)
    {
        SDL_Log("Couldn't create text m_TetrisText");
        return;
    }
}

float valueToLerp = {};
float maxLerpTime = 1.7f;

float alphaTimer = {};

auto StateSetup::OnStateEnter() -> void
{
    auto mainTheme = ResourceManager::GetFromCache<AudioTrack>({ResourceType::Audio, "MainMenu"});
    
    if (auto valueOfMainTheme = mainTheme.value())
    {
        valueOfMainTheme->Play(true);
    }
}

auto StateSetup::OnStateUpdate(float deltaTime) -> std::shared_ptr<IState>
{
    const auto fontShader = ResourceManager::GetFromCache<Shader>({ResourceType::Shader, "FontShader"});

    m_NintendoText->SetColor({0xff, 0x0, 0x0, 0xff});

    m_ScrollTextTimer += deltaTime;
    if (m_ScrollTextTimer <= maxLerpTime)
    {
        valueToLerp = std::lerp(0.0f, (m_Window.Props().height / 2) - (m_NintendoText->GetSize().y / 2) - 100, m_ScrollTextTimer / maxLerpTime);
    }
    else
    {
        alphaTimer += deltaTime;

        if (Input::IsKeyPressed(m_Window, SDLK_SPACE))
        {
            OnStateExit();
            auto& nextState = GetState(StateType::Play);
            nextState->OnStateEnter();
            return nextState;
        }

        const auto textAlpha = static_cast<uint8_t>(std::max<float>(1.0f, std::fabs(0xFF * std::sin(2*alphaTimer))));
        m_PressToSkipText->SetColor({0xff, 0x0, 0x0, textAlpha});

        m_PressToSkipText->Update("PRESS SPACE TO CONTINUE.");
        m_PressToSkipText->Draw(fontShader.value(), (m_Window.Props().width / 2) - (m_PressToSkipText->GetSize().x / 2), (m_Window.Props().height / 2) + 50.0f);
    }

    m_NintendoText->Update("Nintendo©");
    m_NintendoText->Draw(fontShader.value(), (m_Window.Props().width / 2) - (m_NintendoText->GetSize().x / 2), valueToLerp);

    return shared_from_this();
}

auto StateSetup::OnStateExit() -> void
{
    if (const auto mainMenuTheme = ResourceManager::GetFromCache<AudioTrack>({ResourceType::Audio, "MainMenu"}).value())
    {
        mainMenuTheme->Stop();
    }
}