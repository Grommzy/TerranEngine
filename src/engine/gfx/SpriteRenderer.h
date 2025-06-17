#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "engine/core/World.h"
#include "engine/gfx/SpriteBatch.h"

#include <unordered_map>

namespace TerranEngine
{
    class SpriteRenderer : public ISystem
    {
    public:
        SpriteRenderer(::TerranEngine::World& world, const Camera2D& camera) : world(world), camera(camera) {}

        void Update(float deltaTime) override;

    private:
        World& world;
        const Camera2D& camera;
        std::unordered_map<const Texture*, SpriteBatch> batches;
    };
}

#endif // SPRITERENDERER_H
