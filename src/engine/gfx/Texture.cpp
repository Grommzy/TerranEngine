#include "engine/gfx/Texture.h"

#include "engine/core/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <cassert>

namespace TerranEngine
{
    static constexpr GLint PixelFilter = GL_NEAREST;

    Texture::Texture(std::string_view filePath, bool flipY)
    {
        stbi_set_flip_vertically_on_load_thread(flipY);

        int channels = 0;
        unsigned char* data = stbi_load(filePath.data(), &width, &height, &channels, STBI_rgb_alpha);
        
        if (!data)
        {
            TE_LOG_ERROR("Texture load failed: '{}'", filePath);
            return;
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &id);
        glTextureStorage2D(id, 1, GL_RGBA8, width, height);
        glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, PixelFilter);
        glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, PixelFilter);
        glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        stbi_image_free(data);
        TE_LOG_INFO("Loaded texture '{}' ({} x {})", filePath, width, height);
    }

    Texture::Texture(Texture&& otherTexture) noexcept : id(otherTexture.id), width(otherTexture.width), height(otherTexture.height)
    {
        otherTexture.id = 0;
    }

    Texture& Texture::operator=(Texture&& otherTexture) noexcept
    {
        if (this != &otherTexture)
        {
            Release();
            id = otherTexture.id;
            width = otherTexture.width;
            height = otherTexture.height;
            otherTexture.id = 0;
        }

        return *this;
    }

    void Texture::Bind(unsigned int textureSlot) const noexcept
    {
        assert(textureSlot < 32);
        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void Texture::Release() noexcept
    {
        if (id)
        {
            glDeleteTextures(1, &id);
            id = 0;
        }
    }
}