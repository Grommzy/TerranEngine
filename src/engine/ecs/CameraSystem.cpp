#include "engine/ecs/CameraSystem.h"

#include "engine/ecs/world/World.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace TerranEngine
{
    void CameraSystem::Update(World& world, float)
    {
        const int viewportWidth  = windowManager.ViewportWidth();
        const int viewportHeight = windowManager.ViewportHeight();

        world.ForEach<Transform2D, Camera2D>([viewportWidth, viewportHeight](Entity, Transform2D& transform, Camera2D& camera)
        {
            camera.viewportWidth  = viewportWidth;
            camera.viewportHeight = viewportHeight;

            const float snappedX = std::floor(transform.position.x);
            const float snappedY = std::floor(transform.position.y);

            const float left   = snappedX;
            const float right  = snappedX + static_cast<float>(viewportWidth);
            const float bottom = snappedY;
            const float top    = snappedY + static_cast<float>(viewportHeight);

            camera.viewProjection = glm::ortho(left, right, bottom, top);
        });
    }
}