#ifndef TERRANENGINE_TEXTURE_H
#define TERRANENGINE_TEXTURE_H

#include <glad/gl.h>
#include <string_view>

namespace TerranEngine
{
    /**
     * @brief 2D Texture resource with pixel-art defaults in an RGBA-8 colour-space.
     */
    class Texture
    {
    public:
        Texture() = default;
        explicit Texture(std::string_view filePath, bool flipY = true);
        ~Texture() { Release(); }

        Texture(Texture&& otherTexture) noexcept;
        Texture& operator=(Texture&& otherTexture) noexcept;
        Texture(const Texture&)            = delete;
        Texture& operator=(const Texture&) = delete;

        void Bind(unsigned int textureSlot = 0) const noexcept;

        [[nodiscard]] GLuint ID()  const noexcept { return id; }
        [[nodiscard]] int Width()  const noexcept { return width; }
        [[nodiscard]] int Height() const noexcept { return height; }

    private:
        GLuint id  {0};
        int width  {0};
        int height {0};

        void Release() noexcept;
    };
}

#endif // TERRANENGINE_TEXTURE_H