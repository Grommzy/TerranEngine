#ifndef TERRANENGINE_SHADER_H
#define TERRANENGINE_SHADER_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>
#include <string_view>

namespace TerranEngine
{
    class Shader
    {
    public:
        Shader() = default;
        Shader(std::string_view vertPath, std::string_view fragPath);
        ~Shader() { Release(); }

        Shader(Shader&&)            noexcept;
        Shader& operator=(Shader&&) noexcept;
        Shader(const Shader&)           = delete;
        Shader& operator=(const Shader&) = delete;

        void Use() const noexcept { glUseProgram(programID); }

        void SetUniform(std::string_view uniformName, int    value)             const noexcept;
        void SetUniform(std::string_view uniformName, float  value)             const noexcept;
        void SetUniform(std::string_view uniformName, const glm::vec2& vector2) const noexcept;
        void SetUniform(std::string_view uniformName, const glm::vec3& vector3) const noexcept;
        void SetUniform(std::string_view uniformName, const glm::vec4& vector4) const noexcept;
        void SetUniform(std::string_view uniformName, const glm::mat4& matrix4) const noexcept;

        [[nodiscard]] GLuint ID() const noexcept { return programID; }

    private:
        GLuint programID {0};

        mutable std::unordered_map<std::string, GLint> uniformLocationCache;

        [[nodiscard]] GLint GetLocation(std::string_view uniformName) const noexcept;
        static GLuint CompileStage(std::string_view shaderPath, GLenum type);
        void Release() noexcept;
    };
}

#endif // TERRANENGINE_SHADER_H