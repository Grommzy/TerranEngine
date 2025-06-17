#ifndef CAMERA2D_H
#define CAMERA2D_H

#include <glm/glm.hpp>

namespace TerranEngine
{
    class Camera2D
    {
    public:
        Camera2D(int screenWidth, int screenHeight);

        void SetPosition(const glm::vec2& newPosition) noexcept;
        [[nodiscard]] const glm::vec2& Position() const noexcept { return position; }

        void Resize(int newWidth, int newHeight);
        [[nodiscard]] const glm::mat4& Transform() const noexcept { return transform; }

    private:
        void Recalculate();

    private:
        glm::vec2 position  {0.0f}; // World-Pixel coordinates of top-left viewport.
        int screenWidth     {0};
        int screenHeight    {0};
        glm::mat4 transform {1.0f};
    };
}

#endif // CAMERA2D_H