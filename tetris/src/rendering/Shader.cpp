#include <rendering/Shader.h>
#include <Logger.h>

#include <fstream>
#include <source_location>
#include <string_view>
#include <span>
#include <sstream>
#include <format>

auto read_shader(const std::string& path)
{
    auto shaderFile = std::ifstream{ path };
    if (!shaderFile.is_open())
    {
        const auto location = std::source_location::current();
        const auto errorMsg = std::format("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {} [{}:{}]", path, location.file_name(), location.line());
        LOG_CRITICAL(errorMsg);
        // throw std::runtime_error(errorMsg);
    }

    std::stringstream shaderStream = {};
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    return shaderStream.str();
}

auto compile_shader(const std::span<const char> shader_source, const GLenum shader_type)
{
    const auto shader = glCreateShader(shader_type);
    const auto shaderSourcePtr = shader_source.data();
    glShaderSource(shader, 1, &shaderSourcePtr, nullptr);
    glCompileShader(shader);

    GLint success = {};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        GLchar infoLog[512] = {};
        glGetShaderInfoLog(shader, sizeof infoLog, nullptr, infoLog);

        const auto location = std::source_location::current();
        const auto errorMsg = std::format("ERROR::SHADER::COMPILATION_FAILED: [{}:{}]\n{}", location.file_name(), location.line(), infoLog);
        LOG_CRITICAL(errorMsg);
        // throw std::runtime_error(errorMsg);
    }

    return shader;
}

Shader::Shader(const std::string_view vertex, const std::string_view fragment)
{
    const auto vertexShaderCode = read_shader(vertex.data());
    const auto fragmentShaderCode = read_shader(fragment.data());

    const auto vertexId = compile_shader(std::span(vertexShaderCode.data(), vertexShaderCode.size()), GL_VERTEX_SHADER);
    const auto fragmentId = compile_shader(std::span(fragmentShaderCode.data(), fragmentShaderCode.size()), GL_FRAGMENT_SHADER);

    // Shader program
    m_ProgramId = glCreateProgram();
    glAttachShader(m_ProgramId, vertexId);
    glAttachShader(m_ProgramId, fragmentId);
    glLinkProgram(m_ProgramId);

    auto success = GLint{};
    glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &success);
    if (!success) 
    {
        GLchar infoLog[512] = {};
        glGetProgramInfoLog(m_ProgramId, sizeof infoLog, nullptr, infoLog);

        const auto location = std::source_location::current();
        const auto error_message = std::format("ERROR::SHADER::PROGRAM::LINKING_FAILED: [{}:{}]\n{}", location.file_name(), location.line(), infoLog);
        LOG_CRITICAL(error_message);
        // throw std::runtime_error(error_message);
    }

    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);

    LOG_INFO("GLSL Shaders successfully compiled!");
}