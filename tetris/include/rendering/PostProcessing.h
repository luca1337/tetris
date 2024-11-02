#pragma once

#include <Window.h>

#include <functional>
#include <memory>

#include <rendering/Shader.h>
#include <Random.h>
#include <ResourceManager.h>
#include <Types.h>

#include <glad/glad.h>


class PostProcessing
{
public:
    virtual ~PostProcessing() = default;
    PostProcessing(const Window& window);

    virtual inline auto RenderToScene(const float deltaTime, const std::function<void()>& renderSceneHook, const std::function<void()>& renderGuiHook) -> void
    {
        // Step 1: renderizzo la scena di gioco nel framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);

        // renderizzo la scena di gioco (senza GUI)
        renderSceneHook();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        // Step 2: applico il post-processing alla scena di gioco
        #pragma region SHADER_LOGIC
        m_PPShader->Bind();
        #pragma endregion

        glBindVertexArray(m_Vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBindTexture(GL_TEXTURE_2D, m_TextureColorBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Step 3: renderizzo la GUI sopra la scena post-processata per evitare che venga preso in considerazione
        renderGuiHook();
    }

protected:
    const Window& m_Window;
    GLuint m_Vao, m_Vbo, m_Fbo, m_TextureColorBuffer;
    std::shared_ptr<class Shader> m_PPShader = {};
};

class PPChromaticAberration : public PostProcessing
{
public:
    struct PPChromaAberrationData
    {
        float timeElapsed = {};
        float glitchStartTime = {};
        float glitchDuration = {};
        float nextGlitchInterval = {};
        float lastUpdateTime = {};
        float isPaused = {};
    };

public:
    ~PPChromaticAberration() = default;
    PPChromaticAberration(const Window& window) : PostProcessing{window} 
    {
        m_PPShader = std::make_shared<Shader>("../../tetris/resources/shaders/postprocessing/vertex_chroma_aberr.glsl", "../../tetris/resources/shaders/postprocessing/fragment_chroma_aberr.glsl");
        ResourceManager::RegisterResource<Shader>(ResourceParams{ResourceType::Shader, "PPChroma", m_PPShader});
    }

    inline virtual auto RenderToScene(const float deltaTime, const std::function<void()>& renderSceneHook, const std::function<void()>& renderGuiHook) -> void override
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);

        renderSceneHook();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);

#pragma region SHADER_LOGIC
        m_PPShader->Bind();

        m_Data.timeElapsed += deltaTime;

        if (m_Data.timeElapsed > m_Data.lastUpdateTime + m_Data.nextGlitchInterval)
        {
            m_Data.glitchStartTime = m_Data.timeElapsed;
            m_Data.glitchDuration = rng::RNG::GenerateRandomNumber(0.5f, 1.0f);
            m_Data.nextGlitchInterval = rng::RNG::GenerateRandomNumber(2.0f, 5.0f);
            m_Data.lastUpdateTime = m_Data.timeElapsed;
        }

        m_PPShader->SetFloat("time", m_Data.timeElapsed);
        m_PPShader->SetFloat("glitchStartTime", m_Data.glitchStartTime);
        m_PPShader->SetFloat("glitchDuration", m_Data.glitchDuration);
#pragma endregion

        glBindVertexArray(m_Vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBindTexture(GL_TEXTURE_2D, m_TextureColorBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        renderGuiHook();
    }

private:
    PPChromaAberrationData m_Data = {};
};

class PPEdgeDetection : public PostProcessing
{
public:
    ~PPEdgeDetection() = default;
    PPEdgeDetection(const Window& window) : PostProcessing{window} 
    {
        m_PPShader = std::make_shared<Shader>("../../tetris/resources/shaders/postprocessing/vertex_edge_det.glsl", "../../tetris/resources/shaders/postprocessing/fragment_edge_det.glsl");
        ResourceManager::RegisterResource<Shader>(ResourceParams{ResourceType::Shader, "PPEdgeDet", m_PPShader});
    }
};

class PPCCTV : public PostProcessing
{
public:
    ~PPCCTV() = default;
    PPCCTV(const Window& window) : PostProcessing{window} 
    {
        m_PPShader = std::make_shared<Shader>("../../tetris/resources/shaders/postprocessing/vertex_cctv.glsl", "../../tetris/resources/shaders/postprocessing/fragment_cctv.glsl");
        ResourceManager::RegisterResource<Shader>(ResourceParams{ResourceType::Shader, "PPCCTV", m_PPShader});
    }

    inline virtual auto RenderToScene(const float deltaTime, const std::function<void()>& renderSceneHook, const std::function<void()>& renderGuiHook) -> void override
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);

        renderSceneHook();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);

#pragma region SHADER_LOGIC
        m_PPShader->Bind();

        m_Time += deltaTime;

        m_PPShader->SetFloat("time", m_Time);
#pragma endregion

        glBindVertexArray(m_Vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBindTexture(GL_TEXTURE_2D, m_TextureColorBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        renderGuiHook();
    }
private:
    float m_Time = {};
};

class PPEmbossing : public PostProcessing
{
public:
    ~PPEmbossing() = default;
    PPEmbossing(const Window& window) : PostProcessing{window} 
    {
        m_PPShader = std::make_shared<Shader>("../../tetris/resources/shaders/postprocessing/vertex_embossing.glsl", "../../tetris/resources/shaders/postprocessing/fragment_embossing.glsl");
        ResourceManager::RegisterResource<Shader>(ResourceParams{ResourceType::Shader, "PPEmbossing", m_PPShader});
    }
};


/// @brief FOR PAUSE SCREEN ONLY
class PPBlur : public PostProcessing
{
public:
    ~PPBlur() = default;
    PPBlur(const Window& window) : PostProcessing{window} 
    {
        m_PPShader = std::make_shared<Shader>("../../tetris/resources/shaders/postprocessing/vertex_blur.glsl", "../../tetris/resources/shaders/postprocessing/fragment_blur.glsl");
        ResourceManager::RegisterResource<Shader>(ResourceParams{ResourceType::Shader, "PPBlur", m_PPShader});
    }
};
