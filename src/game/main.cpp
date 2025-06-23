#include "engine/core/Log.h"
#include "engine/core/Application.h"
#include "engine/ecs/world/World.h"
#include "engine/ecs/Components.h"
#include "engine/ecs/System.h"
#include "engine/ecs/Behaviour.h"
#include "game/TestBehaviour.h"

int main()
{
    using namespace TerranEngine;

    Application app;

    Entity entity0 = app.GetWorld().CreateEntity();
    Entity entity1 = app.GetWorld().CreateEntity();
    app.GetWorld().AddComponent<Transform2D>(entity0, Transform2D{{0.0f, 0.0f}, {1.0f, 1.0f}, 0});
    app.GetWorld().AddComponent<BehaviourComponent>(entity0, std::make_unique<TestBehaviour>());
    app.GetWorld().AddComponent<Transform2D>(entity1, Transform2D{{1.0f, 0.0f}, {1.0f, 1.0f}, 0});
    app.GetWorld().AddComponent<BehaviourComponent>(entity1, std::make_unique<TestBehaviour>());
    app.GetWorld().DestroyEntity(entity0);
    entity0 = app.GetWorld().CreateEntity();
    app.GetWorld().AddComponent<Transform2D>(entity0, Transform2D{{0.0f, 0.0f}, {1.0f, 1.0f}, 0});
    app.GetWorld().AddComponent<BehaviourComponent>(entity0, std::make_unique<TestBehaviour>());

    app.Run();
    return 0;
}