#include "engine/core/Application.h"
#include "engine/gfx/Shader.h"

int main()
{
    TerranEngine::Application app;

    TerranEngine::Shader shader("../../assets/shaders/test.vert", "../../assets/shaders/test.frag");
    shader.Use();

    while (app.PumpEvents())
    {
        app.BeginFrame();

        /********************* Draw World *********************/
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);   // Magenta test-clear.
        glClear(GL_COLOR_BUFFER_BIT);
        /***************** ^^^ Draw World ^^^ *****************/

        app.EndFrame();
    }
    
    return 0;
}