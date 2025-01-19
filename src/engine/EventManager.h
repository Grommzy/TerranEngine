#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "SDL3/SDL.h"
#include <map>

class Window;   // Forward Declaration of Window.

class EventManager
{
public:
    // Polls SDL for all events in this frame. Returns false if the user has requested to quit.
    bool PollEvents(std::map<SDL_WindowID, Window*>& windows);
private:
    SDL_Event event;
};

#endif // EVENTMANAGER_H