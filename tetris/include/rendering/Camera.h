#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <optional>
#include <memory>

class Camera
{
public:
    Camera() = default;
    Camera(float left, float right, float bottom, float top);

	[[nodiscard]] auto GetPosition() const -> const glm::vec3& { return m_Position; }
	auto SetPosition(const glm::vec3& position) -> void { m_Position = position; }

	[[nodiscard]] auto GetRotation() const -> float { return m_Rotation; }
	auto SetRotation(const float rotation) -> void { m_Rotation = rotation; RecalculateViewMatrix(); }

	[[nodiscard]] auto GetProjectionMatrix() const -> glm::mat4 { return m_ProjectionMatrix; }
	[[nodiscard]] auto GetViewMatrix() const -> glm::mat4 { return m_ViewMatrix; }
	[[nodiscard]] auto GetViewProjectionMatrix() const -> glm::mat4 { return m_ViewProjectionMatrix; }

private:
	auto RecalculateViewMatrix() -> void;

private:
	glm::mat4 m_ProjectionMatrix = {};
	glm::mat4 m_ViewMatrix = {};
	glm::mat4 m_ViewProjectionMatrix = {};

	glm::vec3 m_Position = {};
	float m_Rotation = {};
};