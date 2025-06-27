#include "engine/gfx/SpriteBatch.h"

#include "engine/core/Log.h"

namespace TerranEngine
{
    static constexpr uint16_t INDICES_PER_QUAD  {6};
    static constexpr uint16_t VERTICES_PER_QUAD {4};

    SpriteBatch::SpriteBatch(size_t initialCapacity)
    {
        vertices.reserve(initialCapacity * VERTICES_PER_QUAD);
        indices.reserve(initialCapacity * VERTICES_PER_QUAD);

        glCreateVertexArrays(1, &vao);
        glCreateBuffers(1, &vbo);
        glCreateBuffers(1, &ebo);

        glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
        glVertexArrayElementBuffer(vao, ebo);

        // Position Attribute (X, Y)
        glEnableVertexArrayAttrib(vao, 0);
        glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
        glVertexArrayAttribBinding(vao, 0, 0);

        // UV Attribute (U, V)
        glEnableVertexArrayAttrib(vao, 1);
        glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));
        glVertexArrayAttribBinding(vao, 1, 0);

        // Colour Attribute (R, G, B, A)
        glEnableVertexArrayAttrib(vao, 2);
        glVertexArrayAttribFormat(vao, 2, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, colour));
        glVertexArrayAttribBinding(vao, 2, 0);

        // Depth Attribute (Z)
        glEnableVertexArrayAttrib(vao, 3);
        glVertexArrayAttribFormat(vao, 3, 1, GL_FLOAT, GL_FALSE, offsetof(Vertex, depth));
        glVertexArrayAttribBinding(vao, 3, 0);
    }

    void SpriteBatch::Begin(const Texture& atlas, const Camera2D& camera)
    {
        currentTexture = &atlas;
        shader.Use();
        shader.SetUniform("uTexture0", 0);
        shader.SetUniform("uTransform", camera.viewProjection);

        vertices.clear();
        indices.clear();
        quadCount = 0;
    }

    void SpriteBatch::SubmitQuad(const Vertex* quadVertices)
    {
        Grow(quadCount + 1);

        uint16_t baseIndex = static_cast<uint16_t>(vertices.size());
        vertices.insert(vertices.end(), quadVertices, quadVertices + 4);

        indices.push_back(baseIndex + 0);
        indices.push_back(baseIndex + 1);
        indices.push_back(baseIndex + 2);
        indices.push_back(baseIndex + 0);
        indices.push_back(baseIndex + 2);
        indices.push_back(baseIndex + 3);

        ++quadCount;
    }

    void SpriteBatch::End()
    {
        if (!quadCount) return;

        glNamedBufferData(vbo, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
        glNamedBufferData(ebo, indices.size() * sizeof(uint16_t), indices.data(), GL_DYNAMIC_DRAW);

        currentTexture->Bind(0);
        shader.Use();
        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_SHORT, nullptr);
    }

    void SpriteBatch::Grow(size_t requiredVertices)
    {
        if (requiredVertices <= vertices.capacity() / VERTICES_PER_QUAD) return;

        size_t newCapacity = vertices.capacity() ? vertices.capacity() * 2 : 1024;
        while (newCapacity < requiredVertices * VERTICES_PER_QUAD) { newCapacity *= 2; }

        vertices.reserve(newCapacity);
        indices.reserve((newCapacity / VERTICES_PER_QUAD) * INDICES_PER_QUAD);
    }

    void SpriteBatch::Reset() noexcept { vertices.clear(); indices.clear(); quadCount = 0; }

    SpriteBatch::~SpriteBatch()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }
}