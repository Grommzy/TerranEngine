#ifndef TERRANENGINE_CAMERA2D_H
#define TERRANENGINE_CAMERA2D_H

#include <glm/glm.hpp>

namespace TerranEngine
{
    class Camera2D
    {
    public:
        Camera2D(int targetWidth, int targetHeight);

        void SetPosition(const glm::vec2& newPosition) noexcept;
        [[nodiscard]] const glm::vec2& GetPosition() const noexcept { return worldPosition; }

        void ResizeViewport(int newWidth, int newHeight);
        [[nodiscard]] const glm::mat4 ViewProjectionMatrix() const noexcept { return viewProjectionMatrix; }

    private:
        void RecalculateTransform() noexcept;

    private:
        glm::vec2 worldPosition {0.0f, 0.0f};
        int viewportWidth {0};
        int viewportHeight {0};
        glm::mat4 viewProjectionMatrix {1.0f};
    };
}

#endif // TERRANENGINE_CAMERA2D_H