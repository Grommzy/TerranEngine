#include "engine/core/Log.h"
#include "engine/core/Application.h"
#include "engine/core/Config.h"
#include "engine/ecs/world/World.h"
#include "engine/ecs/Components.h"
#include "engine/ecs/System.h"
#include "engine/ecs/Behaviour.h"
#include "game/TestBehaviour.h"
#include "game/TestCameraBehaviour.h"

int main()
{
    using namespace TerranEngine;

    Config config("Terran Engine", 64, 64, (64 * 10), (64 * 13), false, false, true);

    Application app(config);

    Texture* atlas = new Texture("../../assets/textures/Font Tileset.png");

    Entity camera = app.GetWorld().CreateEntity();
    app.GetWorld().AddComponent<Transform2D>(camera, Transform2D{{0.0f, 0.0f}});
    app.GetWorld().AddComponent<Camera2D>(camera, Camera2D{config.nativeWidth, config.nativeHeight});
    app.GetWorld().AddComponent<BehaviourComponent>(camera, std::make_unique<TestCameraBehaviour>());

    Entity entity0 = app.GetWorld().CreateEntity();
    Entity entity1 = app.GetWorld().CreateEntity();
    app.GetWorld().AddComponent<Transform2D>(entity0, Transform2D{{0.0f, 0.0f}, {1.0f, 1.0f}, 0});
    app.GetWorld().AddComponent<BehaviourComponent>(entity0, std::make_unique<TestBehaviour>());
    app.GetWorld().AddComponent<Transform2D>(entity1, Transform2D{{6.0f, 0.0f}, {1.0f, 1.0f}, 0});
    app.GetWorld().AddComponent<BehaviourComponent>(entity1, std::make_unique<TestBehaviour>());
    app.GetWorld().AddComponent<Sprite>(entity1, Sprite{atlas, {6.0f, 6.0f}, 8});
    app.GetWorld().DestroyEntity(entity0);
    entity0 = app.GetWorld().CreateEntity();
    app.GetWorld().AddComponent<Transform2D>(entity0, Transform2D{{0.0f, 0.0f}, {1.0f, 1.0f}, 0});
    app.GetWorld().AddComponent<Sprite>(entity0, Sprite{atlas, {6.0f, 6.0f}, 7});
    app.GetWorld().AddComponent<BehaviourComponent>(entity0, std::make_unique<TestBehaviour>());

    app.Run();
    return 0;
}