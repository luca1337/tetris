#pragma once

#include <Types.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>

#include <memory>
#include <vector>
#include <cstddef>

struct Particle {
    glm::vec2 m_Position = {};
    glm::vec4 m_Color = {};
    glm::vec2 m_Velocity = {};
    float m_Speed = {};
    float m_LifeTime = {};
    bool m_IsActive = {};
    bool m_ShouldExpire = {};
};

class Shader;
class Texture;

auto DirectionByShapeType(const ShapeType type) -> glm::vec2;

class ParticleSystem
{
    static constexpr float particleQuadVertices[] = {
        0.0f, 1.0f,       0.0f, 1.0f,
        0.0f, 0.0f,       0.0f, 0.0f,
        1.0f, 0.0f,       1.0f, 0.0f,

        0.0f, 1.0f,       0.0f, 1.0f,
        1.0f, 0.0f,       1.0f, 0.0f,
        1.0f, 1.0f,       1.0f, 1.0f 
    };

public:
    ~ParticleSystem() = default;
    ParticleSystem(const glm::vec2 position, const unsigned amount, const float minSpeed, const float maxSpeed, const float minLifeTime, const float maxLifeTime);

    auto SetShapeType(const ShapeType shapeType) { m_ShapeType = shapeType; }

    const auto& Emitting() const { return m_IsEmitting; }
    auto& Emitting() { return m_IsEmitting; }

    const auto& Expire() const { return m_ShouldExpire; }
    auto& Expire() { return m_ShouldExpire; }

    auto SetEmissionTimeInterval(const float interval) -> void;

    auto RandomizeParticlesPositionBetweenBounds(const float xMin, const float xMax, const float yMin, const float yMax) -> void;

    auto SetPosition(const glm::vec2& position) -> void;

    auto Update(float deltaTime) -> void;
    auto Render(const std::shared_ptr<Shader>& shader) -> void;

private:
    std::vector<Particle> m_Particles = {};
    std::vector<Particle> m_ActiveParticles = {};

    GLuint m_QuadVAO, m_QuadVBO, m_InstanceVBO, m_ColorsVBO;

    glm::vec2 m_Position, m_StartPos;
    ShapeType m_ShapeType = {};

    float m_MinSpeed, m_MaxSpeed, m_MinLifeTime, m_MaxLifeTime, m_EmissionTimer;
    float m_EmissionInterval = 0.004f;
    bool m_IsEmitting, m_ShouldExpire;
    unsigned m_Amount = {};

    std::shared_ptr<Texture> m_Texture = {};

private:
    auto GenerateBuffers() -> void;
    auto InitializeParticles() -> void;
    auto ActivateOrCreateParticle() -> void;
    auto UpdateInstanceBuffer(std::vector<Particle>& particles) -> void;
};