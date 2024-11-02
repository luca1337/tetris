#pragma once

#include <IResource.h>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <string_view>

class Shader final : public IResource
{
public:
    Shader() = default;
    Shader(const std::string_view vertex, const std::string_view fragment);

    auto Bind() const { glUseProgram(m_ProgramId); }
    auto Unbind() const { glUseProgram(0); }

    auto SetMatrix4x4(const std::string_view name, const glm::mat4& m) { glUniformMatrix4fv(glGetUniformLocation(m_ProgramId, name.data()), 1, false, &m[0][0]); }
    auto SetFloat(const std::string_view name, const float value) { glUniform1f(glGetUniformLocation(m_ProgramId, name.data()), value); }
    auto SetVec3(const std::string_view name, const glm::vec3& value) { glUniform3fv(glGetUniformLocation(m_ProgramId, name.data()), 1, &value[0]); }
    auto SetVec2(const std::string_view name, const glm::vec2& value) { glUniform2fv(glGetUniformLocation(m_ProgramId, name.data()), 1, &value[0]); }
    auto SetInt(const std::string_view name, const int value) { glUniform1i(glGetUniformLocation(m_ProgramId, name.data()), value); }
    auto SetUint(const std::string_view name, const uint32_t value) { glUniform1ui(glGetUniformLocation(m_ProgramId, name.data()), value); }
    auto SetBool(const std::string_view name, const bool value) { glUniform1i(glGetUniformLocation(m_ProgramId, name.data()), value); }

    auto GetID() const { return m_ProgramId; }

private:
    GLuint m_ProgramId = {};
};