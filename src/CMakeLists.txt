add_library(terranengine_engine STATIC)
add_library(terranengine_game   INTERFACE)

target_sources(terranengine_engine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/core/Log.h
)

target_include_directories(
    terranengine_engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/engine
        ${stb_SOURCE_DIR}       # STB headers
)

# Transitive third-party dependencies
target_link_libraries(
    terranengine_engine
    PUBLIC
        SDL3::SDL3
        glad_gl_core_43
        glm::glm
)

# Create the executable
add_executable(terranengine_demo
    game/main.cpp
)

target_link_libraries(
    terranengine_demo
    PRIVATE
        terranengine_engine
)