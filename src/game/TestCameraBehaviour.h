#ifndef TESTCAMERABEHAVIOUR_H
#define TESTCAMERABEHAVIOUR_H

#include "engine/ecs/Behaviour.h"
#include "engine/ecs/world/World.h"
#include "engine/ecs/Components.h"

class TestCameraBehaviour : public TerranEngine::Behaviour
{
public:
    void Awake() override
    {
        transform = GetWorld().GetComponent<TerranEngine::Transform2D>(GetEntity());
    }

    void Update(float deltaTime) override
    {
        transform->position.y -= 10.0f * deltaTime;
        //TE_LOG_DEBUG("Entity Index '{}' | Generation '{}' at position.x '{}'", entity.Index(), entity.Generation(), transform->position.x);
    }

private:
    TerranEngine::Transform2D* transform;
};

#endif // TESTCAMERABEHAVIOUR_H