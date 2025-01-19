#include "engine/EventManager.h"

#include "engine/Window.h"

bool EventManager::PollEvents(std::map<SDL_WindowID, Window*>& windows)
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                Window* eventWindow = windows[event.window.windowID];
                SDL_DestroyWindow(eventWindow->GetSDLWindow());
                windows.erase(event.window.windowID);

                if (windows.empty())
                {
                    return false;
                }
            break;
        }
    }

    return true;
}