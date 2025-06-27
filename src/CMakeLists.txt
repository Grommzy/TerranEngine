add_library(terranengine_engine STATIC)
add_library(terranengine_game   INTERFACE)

target_sources(terranengine_engine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/core/Time.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/core/Application.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/engine/ecs/world/EntityManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/ecs/world/SystemScheduler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/ecs/ScriptSystem.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/engine/gfx/Texture.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/gfx/Shader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/gfx/Camera2D.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/gfx/SpriteBatch.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/gfx/SpriteRenderer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/gfx/WindowManager.cpp
)

target_include_directories(
    terranengine_engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
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

if(WIN32)
    target_link_libraries(terranengine_engine PUBLIC opengl32)
elseif(APPLE)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)
    target_link_libraries(terranengine_engine PUBLIC ${OPENGL_LIBRARY})
else() # Linux / BSD
    find_package(OpenGL REQUIRED)
    target_link_libraries(terranengine_engine PUBLIC OpenGL::GL)
endif()

# Create the executable
add_executable(terranengine_demo
    game/main.cpp
)

target_link_libraries(
    terranengine_demo
    PRIVATE
        terranengine_engine
)