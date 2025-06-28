#ifndef TERRANENGINE_SPRITEBATCH_H
#define TERRANENGINE_SPRITEBATCH_H

#include "engine/gfx/Shader.h"
#include "engine/gfx/Texture.h"
#include "engine/ecs/components/Components.h"

#include <glm/glm.hpp>
#include <glad/gl.h>
#include <vector>

namespace TerranEngine
{

    struct Vertex
    {
        glm::vec2 position;
        glm::vec2 uv;
        glm::vec4 colour;
        float     depth;
    };

    class SpriteBatch
    {
    public:
        explicit SpriteBatch(size_t initialCapacity = 1024);
        ~SpriteBatch();

        void Begin(const Texture& atlas, const Camera2D& camera);
        void SubmitQuad(const Vertex* vertices);
        void End();
        void Reset() noexcept;

        [[nodiscard]] size_t SpriteCount() const noexcept { return quadCount; }

    private:
        void Grow(size_t requiredVertices);

        GLuint vao {0};
        GLuint vbo {0};
        GLuint ebo {0};

        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;

        const Texture* currentTexture {nullptr};
        Shader shader {"../../assets/shaders/sprite.vert", "../../assets/shaders/sprite.frag"};

        size_t quadCount {0};
    };
}

#endif // TERRANENGINE_SPRITEBATCH_H