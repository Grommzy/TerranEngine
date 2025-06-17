#include "engine/core/Application.h"
#include "engine/core/Time.h"
#include "engine/core/Log.h"
#include "engine/gfx/SpriteBatch.h"
#include "engine/gfx/Texture.h"
#include "engine/gfx/Camera2D.h"
#include "engine/gfx/AtlasUtil.h"
#include "engine/math/Grid.h"
#include "engine/gfx/SpriteRenderer.h"
#include "engine/gfx/AnimationSystem.h"
#include "engine/level/TileGrid.h"
#include "engine/level/TileParser.h"

#include "engine/ecs/Registry.h"
#include "engine/ecs/Entity.h"
#include "engine/ecs/Components.h"

#include "glad/gl.h"

int main()
{
    TerranEngine::Time        time;
    TerranEngine::Application app;
    TerranEngine::World       world;

    TerranEngine::TileGrid tilegrid;
    TerranEngine::Texture atlas("../../assets/textures/Template Tileset.png");
    TerranEngine::Camera2D camera(480, 270); // Native resolution.

    // <Debug Scene Setup>

    TerranEngine::TileParser& tileParser = world.AddSystem<TerranEngine::TileParser>(tilegrid, world.GetRegistry(), atlas, 16);
    tileParser.Load("../../assets/maps/demo.map");
    world.AddSystem<TerranEngine::AnimationSystem>(world);
    world.AddSystem<TerranEngine::SpriteRenderer>(world, camera);

    TerranEngine::Registry& ecs = world.GetRegistry();

    TerranEngine::Entity circle = ecs.CreateEntity();
    ecs.Add<TerranEngine::Transform2D>(circle, TerranEngine::GridToWorld({ 3, 2 }));
    ecs.Add<TerranEngine::SpriteComponent>(circle, &atlas);
    ecs.Get<TerranEngine::SpriteComponent>(circle).atlasIndex = 18;
    ecs.Get<TerranEngine::SpriteComponent>(circle).tint = {0.0f, 0.0f, 1.0f, 1.0f};
    ecs.Add<TerranEngine::AnimationComponent>(circle);
    ecs.Get<TerranEngine::AnimationComponent>(circle).frames = {18, 19, 20, 21};
    ecs.Get<TerranEngine::AnimationComponent>(circle).frameTime = 0.15f;

    TerranEngine::Entity triangle = ecs.CreateEntity();
    ecs.Add<TerranEngine::Transform2D>(triangle, TerranEngine::GridToWorld({ 1, 2 }));
    ecs.Add<TerranEngine::SpriteComponent>(triangle, &atlas);
    ecs.Get<TerranEngine::SpriteComponent>(triangle).atlasIndex = 22;
    ecs.Get<TerranEngine::SpriteComponent>(triangle).tint = {1.0f, 0.0f, 0.0f, 1.0f};
    // </Debug Scene Setup>

    while (app.PumpEvents())
    {
        time.StartFrame();

        // Fixed-tick logic.
        while (time.StepFixed())
        {
            ecs.Get<TerranEngine::Transform2D>(circle).position.x += 0.5f;
            // TE_LOG_INFO("FPS: {}", time.FPS());
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        const bool* keys = SDL_GetKeyboardState(nullptr);
        glm::vec2 camPos = camera.Position();
        if (keys[SDL_SCANCODE_A]) camPos.x -= 100 * app.DeltaTime();
        if (keys[SDL_SCANCODE_D]) camPos.x += 100 * app.DeltaTime();
        if (keys[SDL_SCANCODE_W]) camPos.y -= 100 * app.DeltaTime();
        if (keys[SDL_SCANCODE_S]) camPos.y += 100 * app.DeltaTime();
        camera.SetPosition(camPos);

        app.BeginFrame();

        /********************* Draw World *********************/
        world.Update(time.DeltaTime());
        /***************** ^^^ Draw World ^^^ *****************/

        app.EndFrame();
    }
    
    return 0;
}