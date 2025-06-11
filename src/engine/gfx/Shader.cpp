#include "engine/gfx/Shader.h"
#include "engine/core/Log.h"

#include <fstream>
#include <sstream>
#include <vector>

namespace TerranEngine
{
    Shader::Shader(std::string_view vertexPath, std::string_view fragmentPath)
    {
        const std::string vertexSource   = LoadFile(vertexPath);
        const std::string fragmentSource = LoadFile(fragmentPath);

        const GLuint vertex   = CompileStage(GL_VERTEX_SHADER,     vertexSource);
        const GLuint fragment = CompileStage(GL_FRAGMENT_SHADER, fragmentSource);

        programID = glCreateProgram();
        glAttachShader(programID,   vertex);
        glAttachShader(programID, fragment);
        glLinkProgram(programID);

        GLint linked;
        glGetProgramiv(programID, GL_LINK_STATUS, &linked);
        if (!linked)
        {
            GLint length = 0;
            glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
            std::vector<GLchar> log(length);
            glGetProgramInfoLog(programID, length, nullptr, log.data());
            TE_LOG_ERROR("Shader link error:\n{}", log.data());
            std::abort();
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        TE_LOG_INFO("Shader created: V='{}' F='{}'", vertexPath, fragmentPath);
    }

    Shader::~Shader()
    {
        if (programID) { glDeleteProgram(programID); }
    }

    void Shader::Use() const noexcept
    {
        glUseProgram(programID);
    }

    /************************************** Private Helpers **************************************/

    GLuint Shader::CompileStage(GLenum type, std::string_view source)
    {
        const char* sourcePointer = source.data();
        const GLint length        = static_cast<GLint>(source.size());

        GLuint id = glCreateShader(type);
        glShaderSource(id, 1, &sourcePointer, &length);
        glCompileShader(id);

        GLint compiled;
        glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            GLint logLength = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<GLchar> log(logLength);
            glGetShaderInfoLog(id, logLength, nullptr, log.data());
            TE_LOG_ERROR("{} compile error:\n{}", (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"), log.data());
            std::abort();
        }

        return id;
    }

    std::string Shader::LoadFile(std::string_view path)
    {
        std::ifstream file { std::string {path} };
        if (!file)
        {
            TE_LOG_ERROR("Failed to open shader file '{}'", path);
            std::abort();
        }

        std::stringstream stringStream;
        stringStream << file.rdbuf();

        return stringStream.str();
    }

    GLint Shader::GetUniformLocation(const char* name) const
    {
        const GLint location = glGetUniformLocation(programID, name);

        if (location == -1)
        {
            TE_LOG_WARN("Uniform '{}' not found in shader {}", name, programID);
        }

        return location;
    }

    /************************************** Uniform Helpers **************************************/

    void Shader::SetUniform(const char* name, int value) const
    {
        glUniform1i(GetUniformLocation(name), value);
    }
    
    void Shader::SetUniform(const char* name, float value) const
    {
        glUniform1f(GetUniformLocation(name), value);
    }

    void Shader::SetUniform(const char* name, const glm::vec2& vector2) const
    {
        glUniform2fv(GetUniformLocation(name), 1, &vector2.x);
    }

    void Shader::SetUniform(const char* name, const glm::vec3& vector3) const
    {
        glUniform3fv(GetUniformLocation(name), 1, &vector3.x);
    }

    void Shader::SetUniform(const char* name, const glm::vec4& vector4) const
    {
        glUniform4fv(GetUniformLocation(name), 1, &vector4.x);
    }

    void Shader::SetUniform(const char* name, const glm::mat4& matrix4) const
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix4[0][0]);
    }
}