#include "engine/gfx/SpriteRenderer.h"

#include "engine/ecs/Components.h"
#include "engine/gfx/AtlasUtil.h"

namespace TerranEngine
{
    void SpriteRenderer::Update(float /*deltaTime*/)
    {
        auto& registry = world.GetRegistry();

        registry.ForEach<Transform2D, SpriteComponent>([&](Entity, const Transform2D& transform, const SpriteComponent& spriteComponent)
            {
                if (spriteComponent.texture == nullptr)
                {
                    return;
                }

                SpriteBatch& spriteBatch = batches[spriteComponent.texture];

                if (spriteBatch.SpriteCount() == 0u)
                {
                    spriteBatch.Begin(*spriteComponent.texture, camera);
                }

                Sprite sprite;
                sprite.position = transform.position;
                sprite.scale    = transform.scale;
                sprite.rotation = transform.rotation;
                sprite.size     = spriteComponent.size;
                sprite.anchor   = spriteComponent.anchor;
                sprite.origin   = spriteComponent.origin;
                sprite.tint     = spriteComponent.tint;
                sprite.zLevel   = spriteComponent.zLevel;

                const int columns = spriteComponent.texture->Width() / static_cast<int>(spriteComponent.size.x);
                const int tileX   = spriteComponent.atlasIndex % columns;
                const int tileY   = spriteComponent.atlasIndex / columns;

                sprite.uvRect = TileUV(tileX, tileY, static_cast<int>(spriteComponent.size.x), spriteComponent.texture->Width(), spriteComponent.texture->Height());

                spriteBatch.Submit(sprite);
            });

        for (auto& batchPair : batches)
        {
            SpriteBatch& spriteBatch = batchPair.second;
            spriteBatch.End();
            spriteBatch.Draw();
            spriteBatch.Reset();
        }
    }
}
