#ifndef SHADER_H
#define SHADER_H

#include <string_view>

#include <glad/gl.h>
#include <glm/glm.hpp>

namespace TerranEngine
{
    class Shader
    {
    public:
         Shader(std::string_view vertexPath, std::string_view fragmentPath);
        ~Shader();

        // Ensure that the shader is non-copyable
        Shader(const Shader&)            = delete;
        Shader& operator=(const Shader&) = delete;

        void Use() const noexcept;

        /************************************** Uniform Helpers **************************************/
        void SetUniform(const char* name, int   value) const;
        void SetUniform(const char* name, float value) const;
        void SetUniform(const char* name, const glm::vec2& vector2) const;
        void SetUniform(const char* name, const glm::vec3& vector3) const;
        void SetUniform(const char* name, const glm::vec4& vector4) const;
        void SetUniform(const char* name, const glm::mat4& matrix4) const;

        [[nodiscard]] GLuint ID() const noexcept { return programID; }

    private:
        GLuint CompileStage(GLenum type, std::string_view source);
        std::string LoadFile(std::string_view path);

        GLint GetUniformLocation(const char* name) const;

    private:
        GLuint programID {0};
    };
} // TerranEngine namespace.

#endif // SHADER_H