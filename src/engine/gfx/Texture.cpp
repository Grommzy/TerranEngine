#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

extern "C" const char* stbi_failure_reason();  // Shut up IntelliSense flagging >_<

#include "engine/gfx/Texture.h"
#include "engine/core/Log.h"

#include <utility> // std::exchange

namespace TerranEngine
{
    Texture::Texture(std::string_view path, bool flipVertically)
    {
        stbi_set_flip_vertically_on_load_thread(flipVertically);

        int channels = 0;
        unsigned char* pixels = stbi_load(path.data(), &width, &height, &channels, STBI_rgb_alpha);

        if (!pixels)
        {
            TE_LOG_ERROR("stb_image failed to load '{}': {}", path, stbi_failure_reason());
            std::abort();
        }

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

        stbi_image_free(pixels);

        TE_LOG_INFO("Texture loaded '{}' ({}x{}, RGBA8)", path, width, height);
    }

    Texture::~Texture()
    {
        if (textureID) { glDeleteTextures(1, &textureID); }
    }

    Texture::Texture(Texture&& otherTexture) noexcept
    {
        textureID = std::exchange(otherTexture.textureID, 0);
        width     = std::exchange(otherTexture.width,     0);
        height    = std::exchange(otherTexture.height,    0);
    }

    Texture& Texture::operator=(Texture&& otherTexture) noexcept
    {
        if (this != &otherTexture)
        {
            glDeleteTextures(1, &textureID);

            textureID = std::exchange(otherTexture.textureID, 0);
            width     = std::exchange(otherTexture.width,     0);
            height    = std::exchange(otherTexture.height,    0);
        }

        return *this;
    }

    void Texture::Bind(GLuint slot) const noexcept
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
} // TerranEngine namespace.