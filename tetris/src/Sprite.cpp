#include <Sprite.h>
#include <game/Game.h>
#include <rendering/Shader.h>
#include <rendering/Camera.h>
#include <Texture.h>
#include <GLUtils.h>

#include <vector>
#include <glad/glad.h>

const std::vector<float> quadVertices = {
    0.0f, 1.0f,       0.0f, 1.0f,
    0.0f, 0.0f,       0.0f, 0.0f,
    1.0f, 0.0f,       1.0f, 0.0f,

    0.0f, 1.0f,       0.0f, 1.0f,
    1.0f, 0.0f,       1.0f, 0.0f,
    1.0f, 1.0f,       1.0f, 1.0f 
};

Sprite::~Sprite()
{
    glDeleteVertexArrays(1, &m_Vao);
    glDeleteBuffers(1, &m_Vbo);
}

Sprite::Sprite(const Sprite& other)
{
    glutils::GenerateVertexBuffer(m_Vao, m_Vbo, quadVertices);
    glutils::UploadVertexAttribute(0, 2, 4 * sizeof(float), 0, nullptr);
    glutils::UploadVertexAttribute(1, 2, 4 * sizeof(float), 1, reinterpret_cast<void*>(2 * sizeof(float)));

    m_Texture = other.m_Texture;
    m_Size = other.m_Size;
    m_Position = other.m_Position;
    m_Alpha = other.m_Alpha;
}

Sprite::Sprite()
{
    glutils::GenerateVertexBuffer(m_Vao, m_Vbo, quadVertices);
    glutils::UploadVertexAttribute(0, 2, 4 * sizeof(float), 0, nullptr);
    glutils::UploadVertexAttribute(1, 2, 4 * sizeof(float), 1, reinterpret_cast<void*>(2 * sizeof(float)));
}

Sprite::Sprite(const std::string &texturePath)
{
    glutils::GenerateVertexBuffer(m_Vao, m_Vbo, quadVertices);
    glutils::UploadVertexAttribute(0, 2, 4 * sizeof(float), 0, nullptr);
    glutils::UploadVertexAttribute(1, 2, 4 * sizeof(float), 1, reinterpret_cast<void*>(2 * sizeof(float)));

    m_Texture = std::make_shared<Texture>(texturePath);
    m_Size = { m_Texture->GetWidth(), m_Texture->GetHeight() };
}

void Sprite::Render(const std::shared_ptr<Shader> &shader)
{
    // First bind the shader resource
    shader->Bind();

    if (m_Texture)
    {
        const auto textureId = m_Texture->GetID();
        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    auto& game = Game::GetInstance();

    const auto model = GetLocalModelMatrix();
    const auto view = game.GetMainCamera().GetViewMatrix();
    const auto projection = game.GetMainCamera().GetProjectionMatrix();

    shader->SetMatrix4x4("model", model);
    shader->SetMatrix4x4("view", view);
    shader->SetMatrix4x4("projection", projection);

    shader->SetFloat("alpha", m_Alpha);

    glBindVertexArray(m_Vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindTexture(GL_TEXTURE_2D, 0);

    shader->Unbind();
}

auto Sprite::GetLocalModelMatrix() const -> glm::mat4
{
    const auto translationMatrix = glm::translate(glm::vec3(m_Position, 0.0f));
    const auto rotationMatrix = glm::rotate(0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    const auto scaleMatrix = glm::scale(glm::vec3(m_Size.x, m_Size.y, 1.0f));
    const auto localTransform = translationMatrix * rotationMatrix * scaleMatrix;
    return localTransform;
}
