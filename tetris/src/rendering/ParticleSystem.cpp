#include <rendering/ParticleSystem.h>

#include <rendering/Shader.h>
#include <rendering/Camera.h>
#include <game/Game.h>
#include <Random.h>
#include <Logger.h>
#include <GLUtils.h>
#include <Texture.h>

auto DirectionByShapeType(const ShapeType type) -> glm::vec2
{
    auto direction = glm::vec2{};

    switch (type)
    {
    case ShapeType::HalfCircle:
    {
        const auto randomAngle = rng::RNG::GenerateRandomNumber<float>(-glm::pi<float>(), 2 * glm::pi<float>());
        direction = { glm::cos(randomAngle), glm::sin(randomAngle) };
    }
    break;
    case ShapeType::Circle:
    {
        const auto randomAngle = rng::RNG::GenerateRandomNumber<float>(0.0f, 2 * glm::pi<float>());
        direction = { glm::cos(randomAngle), glm::sin(randomAngle) };
    }
    break;
    case ShapeType::None:
    direction = { rng::RNG::GenerateRandomNumber(-1.0f, 1.0f), rng::RNG::GenerateRandomNumber(-1.0f, 1.0f) };
    break;
    default:
        break;
    }

    return direction;
}


ParticleSystem::ParticleSystem(const glm::vec2 position, const unsigned amount, const float minSpeed, const float maxSpeed, const float minLifeTime, const float maxLifeTime)
    : m_Amount{amount}, m_QuadVAO{}, m_QuadVBO{}, m_InstanceVBO{}, m_ColorsVBO{}, m_StartPos{position}, m_ShapeType{ShapeType::Circle},
      m_MinSpeed{minSpeed}, m_MaxSpeed{maxSpeed}, m_MinLifeTime{minLifeTime}, m_MaxLifeTime{maxLifeTime}, m_IsEmitting{}, m_ShouldExpire{}, m_Position{position}
{
    m_Particles.resize(amount);
    m_ActiveParticles.resize(amount);

    GenerateBuffers();
}

auto ParticleSystem::SetEmissionTimeInterval(const float interval) -> void
{
    constexpr auto minEmissionInterval = 0.004f;
    m_EmissionInterval = glm::max(minEmissionInterval, interval);
}

auto ParticleSystem::RandomizeParticlesPositionBetweenBounds(const float xMin, const float xMax, const float yMin, const float yMax) -> void
{
    for (auto &particle : m_Particles)
    {
        const auto newX = m_StartPos.x + rng::RNG::GenerateRandomNumber(xMin, xMax);
        const auto newY = m_StartPos.y + rng::RNG::GenerateRandomNumber(yMin, yMax);
        particle.m_Position = glm::vec2{newX, newY};
    }

    UpdateInstanceBuffer(m_Particles);
}

auto ParticleSystem::SetPosition(const glm::vec2& position) -> void
{
    m_StartPos = position;

    for (auto &particle : m_Particles)
    {
        particle.m_Position = position;
    }

    UpdateInstanceBuffer(m_Particles);
}

auto ParticleSystem::Update(float deltaTime) -> void
{
    if (!m_IsEmitting) { return; }

    m_EmissionTimer += deltaTime;

    for (unsigned i = 0ul; i != m_Amount; ++i)
    {
        ActivateOrCreateParticle();
    }

    std::erase_if(m_ActiveParticles, [deltaTime](auto& particle) {
        if (particle.m_IsActive) {
            particle.m_LifeTime -= deltaTime;

            if (particle.m_LifeTime > 0.0f) {
                particle.m_Position += particle.m_Velocity * deltaTime * particle.m_Speed;
                return false;
            }
        }
        return true;
    });

    UpdateInstanceBuffer(m_ActiveParticles);
}

auto ParticleSystem::Render(const std::shared_ptr<Shader> &shader) -> void
{
    if (!m_IsEmitting) { return; }

    shader->Bind();

    if (m_Texture)
    {
        const auto textureId = m_Texture->GetID();
        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    // Get view and projection matrices from the game instance
    const auto &game = Game::GetInstance();
    const auto view = game.GetMainCamera().GetViewMatrix();
    const auto projection = game.GetMainCamera().GetProjectionMatrix();

    shader->SetMatrix4x4("view", view);
    shader->SetMatrix4x4("projection", projection);

    glBindVertexArray(m_QuadVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_ActiveParticles.size());
    glBindVertexArray(0);
}

auto ParticleSystem::GenerateBuffers() -> void
{
    m_Texture = std::make_shared<Texture>(30, 30, Color{255, 0, 0, 30});

    glGenVertexArrays(1, &m_QuadVAO);
    glGenBuffers(1, &m_QuadVBO);
    glGenBuffers(1, &m_InstanceVBO);
    glGenBuffers(1, &m_ColorsVBO);

    glBindVertexArray(m_QuadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particleQuadVertices), particleQuadVertices, GL_STATIC_DRAW);

    // Vertex positions attribute
    glutils::UploadVertexAttribute(0, 2, 4 * sizeof(float), 0, nullptr);

    // Uvs positions attribute
    glutils::UploadVertexAttribute(1, 2, 4 * sizeof(float), 1, reinterpret_cast<void*>(2 * sizeof(float)));

    InitializeParticles();
    UpdateInstanceBuffer(m_Particles);

    // Set vertex attributes for model matrix (4 vec4s)
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    constexpr auto vec4Size = sizeof(glm::vec4);
    for (auto i = 0ul; i != 4; ++i)
    {
        glEnableVertexAttribArray(2 + i);
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)static_cast<size_t>(i * vec4Size));
        glVertexAttribDivisor(2 + i, 1);
    }

    // Setup instance VBO for colors
    glBindBuffer(GL_ARRAY_BUFFER, m_ColorsVBO);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glVertexAttribDivisor(6, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 
}

auto ParticleSystem::InitializeParticles() -> void
{
    for (auto& p: m_Particles)
    {
        p.m_Position = m_StartPos;
        p.m_Velocity = DirectionByShapeType(m_ShapeType);
        p.m_Speed = rng::RNG::GenerateRandomNumber(m_MinSpeed, m_MaxSpeed);
        p.m_LifeTime = rng::RNG::GenerateRandomNumber(m_MinLifeTime, m_MaxLifeTime);
        p.m_Color = glm::vec4{rng::RNG::GenerateRandomNumber(0.3f, 1.0f), rng::RNG::GenerateRandomNumber(0.3f, 1.0f), rng::RNG::GenerateRandomNumber(0.3f, 1.0f), 1.0f};
    }
}

auto ParticleSystem::ActivateOrCreateParticle() -> void
{
    for (auto& particle : m_Particles)
    {
        if (!particle.m_IsActive && !particle.m_ShouldExpire)
        {
            particle.m_IsActive = true;
            m_ActiveParticles.push_back(particle);
            return;
        }
    }

    if (m_ShouldExpire) { return; }

    // If no inactive particle is found, create a new one
    auto particleToReturn = Particle{};
    particleToReturn.m_IsActive = true;
    particleToReturn.m_LifeTime = rng::RNG::GenerateRandomNumber(m_MinLifeTime, m_MaxLifeTime);
    particleToReturn.m_Position = m_StartPos;
    particleToReturn.m_Velocity = DirectionByShapeType(m_ShapeType);
    particleToReturn.m_Speed = rng::RNG::GenerateRandomNumber(m_MinSpeed, m_MaxSpeed);

    m_Particles.push_back(particleToReturn);

    LOG_INFO("Created new particle, current count: {}", m_Particles.size());
}

template <typename T, typename F>
auto TransformParticles(const std::vector<Particle>& particles, F transformFunc) -> std::vector<T> 
{
    std::vector<T> results(particles.size());
    std::transform(particles.begin(), particles.end(), results.begin(), transformFunc);
    return results;
}

auto ParticleSystem::UpdateInstanceBuffer(std::vector<Particle>& particles) -> void
{
     // Trasforma le posizioni delle particelle in matrici model per gli shaderr
    auto modelMatrices = TransformParticles<glm::mat4>(particles, [&](const auto& particle) {
        return glm::scale(glm::translate(glm::identity<glm::mat4>(), glm::vec3(particle.m_Position, 0.0f)), glm::vec3{1.0, 1.0, 1.0});
    });

    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4), modelMatrices.data(), GL_DYNAMIC_DRAW);

    // Estrai i colori delle particelle
    auto colors = TransformParticles<glm::vec4>(particles, [](const auto& particle) {
        return particle.m_Color;
    });

    glBindBuffer(GL_ARRAY_BUFFER, m_ColorsVBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_DYNAMIC_DRAW);
}