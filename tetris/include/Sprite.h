#pragma once

#include <Color.h>

#include <glm/vec2.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <string>
#include <memory>

class Shader;
class Texture;

class Sprite
{
public:
    ~Sprite();
    Sprite();
    Sprite(const Sprite& other);
    Sprite(const std::string &texturePath);

    void Render(const std::shared_ptr<Shader> &shader);

    void SetColor(const Color &color) { m_Color = color; }

    void SetLocalTranslation(const glm::ivec2 &position) { m_Position = position; }
    void SetLocalTranslation(const int x, const int y) { m_Position = glm::ivec2{x, y}; }

    void SetLocalScale(const glm::vec2 &size) { m_Scale = size; }
    glm::vec2 GetLocalScale() const { return m_Scale; }

    glm::vec2 GetSize() const { return m_Size; }

    void SetAlpha(const float value) { m_Alpha = value; }
    float GetAlpha() const { return m_Alpha; }

    auto GetPositionOnScreen() const { return m_Position; }

    auto GetLocalModelMatrix() const -> glm::mat4;

private:
    unsigned int m_Vao = {};
    unsigned int m_Vbo = {};

    std::shared_ptr<Texture> m_Texture = {};

    Color m_Color = {};
    glm::ivec2 m_Position = {};
    glm::vec2 m_Scale = {};
    glm::vec2 m_Size = {};

    float m_Alpha = 1.0f;
};