#ifndef TESTCAMERABEHAVIOUR_H
#define TESTCAMERABEHAVIOUR_H

#include "engine/core/Input.h"
#include "engine/ecs/Behaviour.h"
#include "engine/ecs/world/World.h"
#include "engine/ecs/components/Components.h"

using namespace TerranEngine;

class TestCameraBehaviour : public Behaviour
{
public:
    void Awake() override
    {
        transform = GetWorld().GetComponent<Transform2D>(GetEntity());
    }

    void Update(float deltaTime) override
    {
        if (Input::IsDown(Key::W)) { transform->position.y += 16.0f * deltaTime; }
        if (Input::IsDown(Key::A)) { transform->position.x -= 16.0f * deltaTime; }
        if (Input::IsDown(Key::S)) { transform->position.y -= 16.0f * deltaTime; }
        if (Input::IsDown(Key::D)) { transform->position.x += 16.0f * deltaTime; }
    }

private:
    Transform2D* transform;
};

#endif // TESTCAMERABEHAVIOUR_H