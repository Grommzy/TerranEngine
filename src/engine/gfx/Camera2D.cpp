#include "engine/gfx/Camera2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace TerranEngine
{
    Camera2D::Camera2D(int screenWidth, int screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight)
    {
        Recalculate();
    }

    void Camera2D::SetPosition(const glm::vec2& newPosition) noexcept
    {
        position = newPosition;
        Recalculate();
    }

    void Camera2D::Resize(int newWidth, int newHeight)
    {
        screenWidth = newWidth;
        screenHeight = newHeight;

        Recalculate();
    }

    void Camera2D::Recalculate()
    {
        // Snap to whole-pixel edges
        const float snappedX = std::floor(position.x);
        const float snappedY = std::floor(position.y);

        // Orthographic projection, left, right, bottom, top in pixel units.
        const float left = snappedX;
        const float right = snappedX + static_cast<float>(screenWidth);
        const float bottom = snappedY + static_cast<float>(screenHeight);
        const float top = snappedY;

        const glm::mat4 proj = glm::ortho(left, right, bottom, top);
        const glm::mat4 view = glm::mat4(1.0f);

        transform = proj * view;
    }
}