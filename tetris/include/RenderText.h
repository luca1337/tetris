#pragma once

#include <string>
#include <Color.h>

#include <SDL_ttf.h>

#include <glm/vec2.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <memory>

struct SDL_Surface;
struct SDL_Texture;
class Shader;

class RenderText
{
public:
    ~RenderText();
    RenderText(const std::string& fontPath, const int fontSize = 16);

    const auto& GetRawText() const { return m_Text; }

    auto SetText(const std::string& text) { m_Text = text; }
    auto SetColor(const Color color) { m_Color = color; }

    auto Update(const std::string& newText) -> void;
    auto Draw(const std::shared_ptr<Shader>& shader, float x, float y) -> void;

    auto GetPosition() const { return m_Position; }
    auto GetSize() const { return glm::vec2{m_Width, m_Height}; }

private:
    SDL_Surface* m_Surface = {};
    TTF_Font* m_Font = {};

private:
    int m_Width = {};
    int m_Height = {};
    Color m_Color = {};
    std::string m_Text = {};
    glm::vec2 m_Position = {};

    unsigned m_Vao = {};
    unsigned m_Vbo = {};
    unsigned m_TextureID = 0;

private:
    auto CreateOpenGLTextureFromSurface() -> void;
    auto SetupRenderData() -> void;
    auto GetModelMatrix() -> glm::mat4;
};