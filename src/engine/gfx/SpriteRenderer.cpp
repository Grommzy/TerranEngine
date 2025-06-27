#include "engine/gfx/SpriteRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

inline glm::vec2 RotateVec2(const glm::vec2& vector, float radians) noexcept
{
    const float cos = std::cos(radians);
    const float sin = std::sin(radians);
    return { vector.x * cos - vector.y * sin, vector.x * sin + vector.y * cos };
}

namespace TerranEngine
{
    SpriteRenderer::SpriteRenderer(const Camera2D& camera) : camera(camera) {}

    void SpriteRenderer::Update(World& world, float)
    {
        // 1. Push the sprite quads for each component into the correct batch.
        world.ForEach<Transform2D, Sprite>([this](Entity, Transform2D& transform, Sprite& sprite)
        {
            if (!sprite.texture) { return; }

            BatchEntry& batchEntry = batchMap[sprite.texture];
            if (!batchEntry.beganThisFrame)
            {
                batchEntry.batch.Begin(*sprite.texture, camera); 
                batchEntry.beganThisFrame = true;
            }

            SubmitSprite(batchEntry, transform, sprite);
        });

        //2. Flush all batches.
        for (auto& [texture, batchEntry] : batchMap)
        {
            if (batchEntry.beganThisFrame)
            {
                batchEntry.batch.End();
                batchEntry.batch.Reset();
                batchEntry.beganThisFrame = false;
            }
        }
    }

    void SpriteRenderer::SubmitSprite(BatchEntry& batchEntry, const Transform2D& transform, const Sprite& sprite)
    {
        const glm::vec2 scaledSize = sprite.size * transform.scale;

        // Atlas UV rectangle.
        const int columns = sprite.texture->Width()  / static_cast<int>(sprite.size.x);

        const int column = static_cast<int>(sprite.atlasIndex % columns);
        const int row    = static_cast<int>(sprite.atlasIndex / columns);

        const float inverseWidth  = 1.0f / static_cast<float>(sprite.texture->Width());
        const float inverseHeight = 1.0f / static_cast<float>(sprite.texture->Height());

        const glm::vec2 uvMin { column * sprite.size.x * inverseWidth, 1.0f - (row + 1) * sprite.size.y * inverseHeight };
        const glm::vec2 uvMax { uvMin.x  + sprite.size.x * inverseWidth, uvMin.y + sprite.size.y * inverseHeight };

        // Local-Space Quad corners.
        glm::vec2 localBL = { (0.0f - sprite.origin.x) * scaledSize.x, (0.0f - sprite.origin.y) * scaledSize.y };
        glm::vec2 localBR = { (1.0f - sprite.origin.x) * scaledSize.x, (0.0f - sprite.origin.y) * scaledSize.y };
        glm::vec2 localTR = { (1.0f - sprite.origin.x) * scaledSize.x, (1.0f - sprite.origin.y) * scaledSize.y };
        glm::vec2 localTL = { (0.0f - sprite.origin.x) * scaledSize.x, (1.0f - sprite.origin.y) * scaledSize.y };

        // Apply rotation.
        if (transform.rotation != 0.0f)
        {
            localBL = RotateVec2(localBL, transform.rotation);
            localBR = RotateVec2(localBR, transform.rotation);
            localTR = RotateVec2(localTR, transform.rotation);
            localTL = RotateVec2(localTL, transform.rotation);            
        }

        // Anchor translation.
        const glm::vec2 anchorOffset = RotateVec2(glm::vec2((sprite.origin.x - sprite.anchor.x) * scaledSize.x, (sprite.origin.y - sprite.anchor.y) * scaledSize.y), transform.rotation);

        const glm::vec2 worldOrigin = transform.position + anchorOffset;

        const glm::vec2 bl = worldOrigin + localBL;
        const glm::vec2 br = worldOrigin + localBR;
        const glm::vec2 tr = worldOrigin + localTR;
        const glm::vec2 tl = worldOrigin + localTL;

        const float depth = static_cast<float>(sprite.zLevel) * 0.01f;

        Vertex quad[4] = 
        {
            { bl, {uvMin.x, uvMin.y}, sprite.tint, depth },
            { br, {uvMax.x, uvMin.y}, sprite.tint, depth },
            { tr, {uvMax.x, uvMax.y}, sprite.tint, depth },
            { tl, {uvMin.x, uvMax.y}, sprite.tint, depth }
        };

        batchEntry.batch.SubmitQuad(quad);
    }
}