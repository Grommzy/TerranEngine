#include "engine/core/Application.h"
#include "engine/gfx/Shader.h"
#include "engine/gfx/Texture.h"

int main()
{
    TerranEngine::Application app;

    TerranEngine::Shader shader("../../assets/shaders/test.vert", "../../assets/shaders/test.frag");
    shader.Use();

    TerranEngine::Texture texture("../../assets/textures/ColourChecker.jpg");
    texture.Bind(0);

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