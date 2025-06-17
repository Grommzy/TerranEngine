#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/gl.h>

#include <string_view>

namespace TerranEngine
{
    class Texture
    {
    public:
        Texture(std::string_view path, bool flipVertically = false);
        ~Texture();

        // Ensure that textures are non-copyable.
        Texture(const Texture&)            = delete;
        Texture& operator=(const Texture&) = delete;

        // Ensure that the textures are movable.
        Texture(Texture&& otherTexture) noexcept;
        Texture& operator=(Texture&& ptherTexture) noexcept;

        void Bind(GLuint slot = 0)  const noexcept;
        [[nodiscard]] GLuint  ID()  const noexcept { return textureID; }
        [[nodiscard]] int  Width()  const noexcept { return width; }
        [[nodiscard]] int Height()  const noexcept { return height; }

    private:
        GLuint textureID {0};
        int width        {0};
        int height       {0};
    };
} // TerranEngine namespace.

#endif // TEXTURE_H