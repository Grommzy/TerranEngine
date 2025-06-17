#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H

#include "engine/gfx/Shader.h"
#include "engine/gfx/Texture.h"
#include "engine/gfx/Camera2D.h"

#include <glm/glm.hpp>
#include <vector>

namespace TerranEngine
{
    struct Sprite
    {
        glm::vec2 position {0.0f};                      // XposPx, YposPx
        glm::vec2 origin   {0.5f, 0.5f};                // 0-1 range, 0,0 = top-left. Effects transformations.
        glm::vec2 anchor   {0.0f, 0.0f};                // 0-1 range, 0,0 = top-left. Effects translations.
        glm::vec2 size     {16.0f};                     // XscalePx, YscalePx (unscaled)
        glm::vec2 scale    {1.0f};                      // Non-uniform scaling factor.
        float     rotation {0.0f};                      // Rotation in radians.
        glm::vec4 uvRect   {0.0f, 0.0f, 1.0f, 1.0f};    // u, v, uSize, vSize
        glm::vec4 tint     {1.0f};                      // RGBA
        int zLevel         {0};                         // Z
    };

    class SpriteBatch
    {
    public:
        SpriteBatch();
        ~SpriteBatch();

        // Ensure that the class is non-copyable.
        SpriteBatch(const SpriteBatch&)            = delete;
        SpriteBatch& operator=(const SpriteBatch&) = delete;

        void Begin(const Texture& textureAtlas, const Camera2D& camera);    // Bind texture atlas and reset the buffer.
        void Submit(const Sprite& sprite);          // Push one sprite.
        void End();                                 // Flush to the GPU.
        void Draw();                                // Issue to the draw call.
        void Reset() noexcept;

        [[nodiscard]] std::size_t SpriteCount() const noexcept { return spriteCount; }

    private:
        void InitGLResources();
        void GrowBuffers(std::size_t minimumCapacity);
        inline glm::vec2 TransformVertex(const Sprite& sprite, const glm::vec2& corner, const glm::vec2& pivotOffset, const glm::vec2& scale, const float cosA, const float sinA) const;

    private:
        struct Vertex
        {
            glm::vec2 position;
            glm::vec2 uv;
            glm::vec4 colour;
            float     depth;
        };

        GLuint vbo {0};
        GLuint vao {0};
        GLuint ebo {0};

        const Camera2D* currentCamera {nullptr};

        std::vector<Vertex>   vertices;
        std::vector<uint16_t> indices;

        Shader shader {"../../assets/shaders/sprite.vert", "../../assets/shaders/sprite.frag"};

        std::size_t spriteCount {0};
    };
}

#endif // SPRITEBATCH_H