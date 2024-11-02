#include <RenderText.h>
#include <rendering/Shader.h>
#include <game/Game.h>
#include <rendering/Camera.h>
#include <GLUtils.h>

#include <glad/glad.h>

#include <vector>

const std::vector<float> quadVertices = 
{
    0.0f, 1.0f,       0.0f, 1.0f,
    0.0f, 0.0f,       0.0f, 0.0f,
    1.0f, 0.0f,       1.0f, 0.0f,

    0.0f, 1.0f,       0.0f, 1.0f,
    1.0f, 0.0f,       1.0f, 0.0f,
    1.0f, 1.0f,       1.0f, 1.0f 
};

RenderText::~RenderText()
{
    if (m_Surface)
    {
        SDL_FreeSurface(m_Surface);
    }

    if (m_TextureID)
    {
        glDeleteTextures(1, &m_TextureID);
    }

    if (m_Font)
    {
        TTF_CloseFont(m_Font);
    }
}

RenderText::RenderText(const std::string &fontPath, const int fontSize)
{
    m_Font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!m_Font)
    {
        SDL_Log("Error on opening font: %s", TTF_GetError());
        return;
    }

    SetupRenderData();

    Update(" ");
}

auto RenderText::Update(const std::string &newText) -> void
{
    SetText(newText);

    if (m_Surface != nullptr)
    {
        SDL_FreeSurface(m_Surface);
        m_Surface = nullptr;
    }

    if (m_TextureID != 0)
    {
        glDeleteTextures(1, &m_TextureID);
        m_TextureID = 0;
    }

    m_Surface = TTF_RenderUTF8_Blended(m_Font, newText.c_str(), SDL_Color{m_Color.r, m_Color.g, m_Color.b, m_Color.a});
    if (m_Surface == nullptr)
    {
        SDL_Log("Failed to create surface: %s", TTF_GetError());
        return;
    }

    CreateOpenGLTextureFromSurface();
}

auto RenderText::Draw(const std::shared_ptr<Shader>& shader, float x, float y) -> void
{
    shader->Bind();

    m_Position = glm::vec2{x, y};

    auto& game = Game::GetInstance();

    const auto model = GetModelMatrix();
    const auto view = game.GetMainCamera().GetViewMatrix();
    const auto projection = game.GetMainCamera().GetProjectionMatrix();

    shader->SetMatrix4x4("model", model);
    shader->SetMatrix4x4("view", view);
    shader->SetMatrix4x4("projection", projection);

    glBindVertexArray(m_Vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    // Render the textured quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto RenderText::CreateOpenGLTextureFromSurface() -> void
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    // Ensure the surface is in the correct format
    const auto formattedSurface = SDL_ConvertSurfaceFormat(m_Surface, SDL_PIXELFORMAT_RGBA32, 0);
    if (!formattedSurface)
    {
        SDL_Log("Failed to convert surface format: %s", SDL_GetError());
        return;
    }

    // Set pixel storage mode
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, formattedSurface->w, formattedSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, formattedSurface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_Width = formattedSurface->w;
    m_Height = formattedSurface->h;
    SDL_FreeSurface(formattedSurface);
    SDL_FreeSurface(m_Surface);
    m_Surface = nullptr;
}

auto RenderText::SetupRenderData() -> void
{
    glutils::GenerateVertexBuffer(m_Vao, m_Vbo, quadVertices);
    glutils::UploadVertexAttribute(0, 2, 4 * sizeof(float), 0, nullptr);
    glutils::UploadVertexAttribute(1, 2, 4 * sizeof(float), 1, reinterpret_cast<void*>(2 * sizeof(float)));
}

auto RenderText::GetModelMatrix() -> glm::mat4
{
    const auto translationMatrix = glm::translate(glm::identity<glm::mat4>(), glm::vec3{m_Position, 0.0f});
    const auto rotationMatrix = glm::rotate(glm::identity<glm::mat4>(), 0.0f, glm::vec3{0.0f, 0.0f, 1.0f});
    const auto scaleMatrix = glm::scale(glm::identity<glm::mat4>(), glm::vec3{m_Width, m_Height, 1.0f});
    return translationMatrix * rotationMatrix * scaleMatrix;
}
