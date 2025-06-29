#ifndef TESTBEHAVIOUR_H
#define TESTBEHAVIOUR_H

#include "engine/core/Log.h"
#include "engine/core/Input.h"
#include "engine/ecs/Behaviour.h"
#include "engine/ecs/world/World.h"
#include "engine/ecs/components/Components.h"

using namespace TerranEngine;

class TestBehaviour : public Behaviour
{
public:
    void Awake() override
    {
        transform = GetWorld().GetComponent<Transform2D>(GetEntity());
        sprite = GetWorld().GetComponent<Sprite>(GetEntity());
    }

    void Update(float deltaTime) override
    {
        transform->position.x += 10.0f * deltaTime;
        if (Input::WasMousePressed(MouseButton::Left))  { sprite->tint = {1.0f, 0.0f, 0.0f, 1.0f}; }
        if (Input::WasMouseReleased(MouseButton::Left)) { sprite->tint = {1.0f, 1.0f, 1.0f, 1.0f}; }
        //TE_LOG_DEBUG("Entity Index '{}' | Generation '{}' at position.x '{}'", entity.Index(), entity.Generation(), transform->position.x);
    }

private:
    Transform2D* transform;
    Sprite* sprite;
};

#endif // TESTBEHAVIOUR_H