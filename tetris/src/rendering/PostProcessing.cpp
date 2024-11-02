#include <rendering/PostProcessing.h>
#include <GLUtils.h>
#include <ResourceManager.h>
#include <rendering/Shader.h>
#include <Random.h>
#include <Window.h>

PostProcessing::PostProcessing(const Window& window) : m_Window{window}, m_Vao{}, m_Vbo{}, m_Fbo{}, m_TextureColorBuffer{}
{
    const auto quadVertices = std::vector<float>
    {
        -1.0f, -1.0f,  0.0f, 0.0f,
        -1.0f, 1.0f,  0.0f, 1.0f,
         1.0f, 1.0f,  1.0f, 1.0f,

        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, 1.0f,  1.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f
    };

    glutils::GenerateVertexBuffer(m_Vao, m_Vbo, quadVertices);
    glutils::UploadVertexAttribute(0, 2, 4 * sizeof(float), 0, nullptr);
    glutils::UploadVertexAttribute(1, 2, 4 * sizeof(float), 1, reinterpret_cast<void*>(2 * sizeof(float)));

    const auto width = m_Window.Props().width;
    const auto height = m_Window.Props().height;

    glutils::GenerateFrameBufferObject(width, height, m_Fbo, m_TextureColorBuffer);
}