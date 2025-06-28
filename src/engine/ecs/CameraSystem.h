#ifndef TERRANENGINE_CAMERASYSTEM_H
#define TERRANENGINE_CAMERASYSTEM_H

#include "engine/gfx/WindowManager.h"
#include "engine/ecs/System.h"
#include "engine/ecs/components/Components.h"

namespace TerranEngine
{
    class CameraSystem final : public System
    {
    public:
        explicit CameraSystem(WindowManager& windowManager) : windowManager(windowManager) {}

        void Update(World& world, float deltaTime) override;

    private:
        WindowManager& windowManager;
    };
}

#endif // TERRANENGINE_CAMERASYSTEM_H