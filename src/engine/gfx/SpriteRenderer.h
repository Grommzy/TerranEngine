#ifndef TERRANENGINE_SPRITERENDERER_H
#define TERRANENGINE_SPRITERENDERER_H

#include "engine/ecs/System.h"
#include "engine/gfx/Camera2D.h"
#include "engine/gfx/SpriteBatch.h"
#include "engine/ecs/Components.h"
#include "engine/ecs/world/World.h"

#include <unordered_map>

namespace TerranEngine
{
    class SpriteRenderer final : public System
    {
    public:
        SpriteRenderer(const Camera2D& camera);

        void Update(World& world, float deltaTime) override;

    private:
        struct BatchEntry
        {
            SpriteBatch batch;
            bool beganThisFrame {false};
        };

        void SubmitSprite(BatchEntry& entry, const Transform2D& transform, const Sprite& sprite);

    private:
        const Camera2D& camera;
        std::unordered_map<const Texture*, BatchEntry> batchMap;
    };
}

#endif // TERRANENGINE_SPRITERENDERER_H