cmake_minimum_required(VERSION 3.25)

project(Ramforge LANGUAGES CXX)

# Put all runtime targets (EXE / DLL) in build/bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Build-Options
include(cmake/Options.cmake) # Defines RF_ENABLE_GL_DEBUG, RF_STATIC_SDL, ...

# Ninja by default (unless generator already chosen)
if (CMAKE_GENERATOR STREQUAL "Unix Makefiles" AND NOT DEFINED ENV{NINJA})
    message(STATUS "Switching to Ninja generator")
    set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "" FORCE)
endif()

# Out-of-source builds only
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source build forbidden. Please run `cmake -B build -S .`")
endif()

#C++20, warnings-as-errors in Debug
set(CMAKE_CXX_STANDARD 20)
if(MSVC)
    add_compile_options(/permissive- /W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(RF_DEBUG=1)
endif()

# Dependencies
include(FetchContent)

# --SDL3 (Dynamic by Default) (Helper module ensures that SDL3's upstream CMake exports only SDL3::SDL3 as static/shared naming conventions differ per platform)
set(RF_SDL_SHARED ON CACHE BOOL "Build/Link SDL3 dynamically")
if(RF_STATIC_SDL)
    set(RF_SDL_SHARED OFF)
endif()

include(cmake/FetchSDL3.cmake) # Guarantees SDL3::SDL3 target

# --GLAD (Single Header + .c file)
FetchContent_Declare(
    glad
    GIT_REPOSITORY  https://github.com/Dav1dde/glad.git
    GIT_TAG         v2.0.5  # Latest stable at time of writing
)

set(GLAD_PROFILE "core")
set(GLAD_API "gl=4.3")
set(GLAD_GENERATOR "c")
FetchContent_MakeAvailable(glad) # GLAD now available as target 'glad'

# --GLM and STB-Image (header-only)
FetchContent_Declare(
    glm
    GIT_REPOSITORY  https://github.com/g-truc/glm.git
    GIT_TAG         1.0.1
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    stb
    GIT_REPOSITORY  https://github.com/nothings/stb.git
    GIT_TAG         master
)
FetchContent_MakeAvailable(stb)

# Source tree placeholders
add_subdirectory(src)