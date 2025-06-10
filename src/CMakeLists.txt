add_library(ramforge_engine INTERFACE) #will be OBJECT/STATIC later
add_library(ramforge_game   INTERFACE)

target_include_directories(
    ramforge_engine
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/engine
        ${stb_SOURCE_DIR}
)

# Transitive third-party dependencies
target_link_libraries(
    ramforge_engine
    INTERFACE
        SDL3::SDL3
        glad
        glm::glm
)