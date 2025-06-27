#include "engine/gfx/Shader.h"
#include "engine/core/Log.h"

#include <fstream>
#include <sstream>

namespace TerranEngine
{
    static std::string LoadFile(std::string_view filePath)
    {
        std::ifstream file(filePath.data(), std::ios::in | std::ios::binary);
        if (!file)
        {
            TE_LOG_ERROR("Cannot open shader '{}'", filePath);
            return {};
        }

        std::ostringstream stringStream;
        stringStream << file.rdbuf();

        return stringStream.str();
    }

    GLuint Shader::CompileStage(std::string_view shaderPath, GLenum type)
    {
        std::string source = LoadFile(shaderPath);
        if (source.empty()) { return 0; }

        GLuint id = glCreateShader(type);
        const char* pointer = source.c_str();
        glShaderSource(id, 1, &pointer, nullptr);
        glCompileShader(id);

        GLint compiled = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            char log[512];
            glGetShaderInfoLog(id, 512, nullptr, log);
            TE_LOG_ERROR("Shader compilation failed ('{}') failed: {}", shaderPath, log);
            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    Shader::Shader(std::string_view vertPath, std::string_view fragPath)
    {
        GLuint vertShader = CompileStage(vertPath, GL_VERTEX_SHADER);
        GLuint fragShader = CompileStage(fragPath, GL_FRAGMENT_SHADER);
        if (!vertShader || !fragShader) { return; }

        programID = glCreateProgram();
        glAttachShader(programID, vertShader);
        glAttachShader(programID, fragShader);
        glLinkProgram(programID);

        GLint linked = 0;
        glGetProgramiv(programID, GL_LINK_STATUS, &linked);
        if (!linked)
        {
            char log[512];
            glGetProgramInfoLog(programID, 512, nullptr, log);
            Release();
            TE_LOG_ERROR("Linking Shader-Program ('{} | {}') failed: {}", vertPath, fragPath, log);
        }

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        TE_LOG_INFO("Shader-Program created: {} | {}", vertPath, fragPath);
    }

    Shader::Shader(Shader&& otherShader) noexcept : programID(otherShader.programID), uniformLocationCache(std::move(otherShader.uniformLocationCache)) { otherShader.programID = 0; }

    Shader& Shader::operator=(Shader&& otherShader) noexcept
    {
        if (this != &otherShader)
        {
            Release();
            programID = otherShader.programID;
            uniformLocationCache = std::move(otherShader.uniformLocationCache);
            otherShader.programID = 0;
        }

        return *this;
    }

    GLint Shader::GetLocation(std::string_view uniformName) const noexcept
    {
        auto iterator = uniformLocationCache.find(std::string(uniformName));
        if (iterator != uniformLocationCache.end()) { return iterator->second; }

        GLint location = glGetUniformLocation(programID, std::string(uniformName).c_str());
        uniformLocationCache.emplace(uniformName, location);

        return location;
    }

    void Shader::Release() noexcept
    {
        if (programID)
        {
            glDeleteProgram(programID);
            programID = 0;
        }
    }

    void Shader::SetUniform(std::string_view uniformName, int value) const noexcept { glUniform1i(GetLocation(uniformName), value); }
    void Shader::SetUniform(std::string_view uniformName, float value) const noexcept { glUniform1f(GetLocation(uniformName), value); }
    void Shader::SetUniform(std::string_view uniformName, const glm::vec2& vertex2) const noexcept { glUniform2fv(GetLocation(uniformName), 1, &vertex2.x); }
    void Shader::SetUniform(std::string_view uniformName, const glm::vec3& vertex3) const noexcept { glUniform3fv(GetLocation(uniformName), 1, &vertex3.x); }
    void Shader::SetUniform(std::string_view uniformName, const glm::vec4& vertex4) const noexcept { glUniform4fv(GetLocation(uniformName), 1, &vertex4.x); }
    void Shader::SetUniform(std::string_view uniformName, const glm::mat4& matrix4) const noexcept { glUniformMatrix4fv(GetLocation(uniformName), 1, GL_FALSE, &matrix4[0][0]); }
}