#ifndef TERRANENGINE_CAMERA2D_H
#define TERRANENGINE_CAMERA2D_H

#include <glm/glm.hpp>

namespace TerranEngine
{
    struct Camera2D
    {
        int targetWidth          {320};
        int targetHeight         {240};
        int viewportWidth        {320};
        int viewportHeight       {240};
        glm::mat4 viewProjection {1.0f};

        bool primary             {true};
    };
}

#endif // TERRANENGINE_CAMERA2D_H