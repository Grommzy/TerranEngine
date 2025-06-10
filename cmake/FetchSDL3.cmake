include(FetchContent)

if(NOT TARGET SDL3::SDL3)
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY  https://github.com/libsdl-org/SDL.git
        GIT_TAG         main
        GIT_SHALLOW     TRUE
    )
    set(SDL_STATIC      ${TE_STATIC_SDL})
    set(SDL_SHARED      ON)     # Both artefacts get built; we link the one we want.
    FetchContent_MakeAvailable(SDL3)
endif()