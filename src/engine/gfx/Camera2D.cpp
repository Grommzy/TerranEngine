#include "engine/gfx/Camera2D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace TerranEngine
{
    Camera2D::Camera2D(int targetWidth, int targetHeight) : viewportWidth(targetWidth), viewportHeight(targetHeight) { RecalculateTransform(); }

    void Camera2D::SetPosition(const glm::vec2& newPosition) noexcept
    {
        worldPosition = newPosition;
        RecalculateTransform();
    }

    void Camera2D::ResizeViewport(int newWidth, int newHeight)
    {
        viewportWidth = newWidth;
        viewportHeight = newHeight;
        RecalculateTransform();
    }

    void Camera2D::RecalculateTransform() noexcept
    {
        // Snap to whole pixels for perfect texel alignment.
        const float snappedX = std::floor(worldPosition.x);
        const float snappedY = std::floor(worldPosition.y);

        // Bottom-left origin.
        const float left   = snappedX;
        const float right  = snappedX + static_cast<float>(viewportWidth);
        const float bottom = snappedY;
        const float top    = snappedY + static_cast<float>(viewportHeight);

        const glm::mat4 projection = glm::ortho(left, right, bottom, top);
        const glm::mat4 view       = glm::mat4(1.0f);
        viewProjectionMatrix       = projection * view;
    }
}