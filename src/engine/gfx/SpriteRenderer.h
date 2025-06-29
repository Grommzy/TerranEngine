#ifndef TERRANENGINE_SPRITERENDERER_H
#define TERRANENGINE_SPRITERENDERER_H

#include "engine/ecs/System.h"
#include "engine/gfx/SpriteBatch.h"
#include "engine/ecs/components/Components.h"
#include "engine/ecs/world/World.h"

#include <unordered_map>

namespace TerranEngine
{
    class SpriteRenderer final : public System
    {
    public:
        SpriteRenderer() = default;

        void Update(World& world, float deltaTime) override;

    private:
        struct BatchEntry
        {
            SpriteBatch batch;
            bool beganThisFrame {false};
        };

        void SubmitSprite(BatchEntry& entry, const Transform2D& transform, const Sprite& sprite);

    private:
        std::unordered_map<const Texture*, BatchEntry> batchMap;
    };
}

#endif // TERRANENGINE_SPRITERENDERER_H