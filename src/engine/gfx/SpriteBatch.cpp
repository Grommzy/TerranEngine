#include "engine/gfx/SpriteBatch.h"
#include "engine/core/Log.h"

#include <glad/gl.h>

namespace TerranEngine
{
    SpriteBatch::SpriteBatch()
    {
        InitGLResources();
    }

    SpriteBatch::~SpriteBatch()
    {
        if (vbo) glDeleteBuffers      (1, &vbo);
        if (ebo) glDeleteBuffers      (1, &ebo);
        if (vao) glDeleteVertexArrays (1, &vao);
    }

    void SpriteBatch::Begin(const Texture& textureAtlas, const Camera2D& camera)
    {
        currentCamera = &camera;

        textureAtlas.Bind(0);
        shader.Use();
        shader.SetUniform("uTexture0", 0);
        shader.SetUniform("uTransform", camera.Transform());

        vertices.clear();
        indices.clear();
        spriteCount = 0;
    }

    void SpriteBatch::Submit(const Sprite& sprite)
    {
        const glm::vec2 size  = sprite.size;
        const glm::vec2 scale = sprite.scale;
        const float     angle = sprite.rotation;
        const float     cosA  = std::cos(angle);
        const float     sinA  = std::sin(angle);

        const glm::vec2 pivotOffset = (sprite.origin - sprite.anchor) * size;

        // 4 vertices per quad:
        glm::vec2 tl = (-sprite.anchor)                       * sprite.size;
        glm::vec2 tr = (glm::vec2{1, 0} - sprite.anchor)      * sprite.size; 
        glm::vec2 bl = (glm::vec2{0, 1} - sprite.anchor)      * sprite.size; 
        glm::vec2 br = (glm::vec2{1, 1} - sprite.anchor)      * sprite.size; 

        glm::vec2 p0 = TransformVertex(sprite, tl, pivotOffset, scale, cosA, sinA);
        glm::vec2 p1 = TransformVertex(sprite, tr, pivotOffset, scale, cosA, sinA);
        glm::vec2 p2 = TransformVertex(sprite, bl, pivotOffset, scale, cosA, sinA);
        glm::vec2 p3 = TransformVertex(sprite, br, pivotOffset, scale, cosA, sinA);

        const glm::vec2 uv0 = { sprite.uvRect.x,                   sprite.uvRect.y };
        const glm::vec2 uv1 = { sprite.uvRect.x + sprite.uvRect.z, sprite.uvRect.y };
        const glm::vec2 uv2 = { sprite.uvRect.x,                   sprite.uvRect.y + sprite.uvRect.w };
        const glm::vec2 uv3 = { sprite.uvRect.x + sprite.uvRect.z, sprite.uvRect.y + sprite.uvRect.w };

        const std::uint16_t base = static_cast<std::uint16_t>(vertices.size());

        vertices.push_back({ p0, uv0, sprite.tint, sprite.zLevel * 0.01f });
        vertices.push_back({ p1, uv1, sprite.tint, sprite.zLevel * 0.01f });
        vertices.push_back({ p2, uv2, sprite.tint, sprite.zLevel * 0.01f });
        vertices.push_back({ p3, uv3, sprite.tint, sprite.zLevel * 0.01f });

        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        indices.push_back(base + 2);
        indices.push_back(base + 1);
        indices.push_back(base + 3);

        ++spriteCount;
    }

    void SpriteBatch::End()
    {
        if (vertices.empty())
        {
            return;
        }

        GrowBuffers(vertices.size());
        glBindVertexArray(vao);

        // Orphan and upload.
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(uint16_t)), indices.data(), GL_DYNAMIC_DRAW);
    }

    void SpriteBatch::Draw()
    {
        if (spriteCount == 0)
        {
            return;
        }

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_SHORT, nullptr);
    }

    void SpriteBatch::Reset() noexcept
    {
        vertices.clear();
        indices .clear();
        spriteCount = 0;
    }

    void SpriteBatch::InitGLResources()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers     (1, &vbo);
        glGenBuffers     (1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        const GLsizei stride = sizeof(Vertex);

        glEnableVertexAttribArray(0); // POSITION (X, Y)
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, position)));

        glEnableVertexAttribArray(1); // UV (U, V)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, uv)));

        glEnableVertexAttribArray(2); // COLOUR (R, G, B, A)
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, colour)));

        glEnableVertexAttribArray(3); // Z-LEVEL (Z)
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, depth)));

        TE_LOG_INFO("SpriteBatch GL resources initialised.");
    }

    void SpriteBatch::GrowBuffers(std::size_t minimumCapacity)
    {
        const std::size_t current = vertices.capacity();
        
        if (minimumCapacity > current)
        {
            const std::size_t newCapacity = std::max(minimumCapacity, current * 2);
            vertices.reserve(newCapacity);
            indices.reserve(newCapacity * 6 / 4); // Reserve 6 incices per quad.
        }
    }

    glm::vec2 SpriteBatch::TransformVertex(const Sprite& sprite, const glm::vec2& corner, const glm::vec2& pivotOffset, const glm::vec2& scale, const float cosA, const float sinA) const
    {
        // shift so pivot is origin
        glm::vec2 vertex = corner - pivotOffset;
        // scale
        vertex *= scale;
        // rotate
        vertex = { vertex.x * cosA - vertex.y * sinA,
              vertex.x * sinA + vertex.y * cosA };
        // shift back, then to world
        return sprite.position + pivotOffset + vertex;
    }
} // TerranEngine namespace.