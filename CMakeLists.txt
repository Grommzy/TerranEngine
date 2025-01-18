###################################################
# [Basic Setup] ###################################
# CMake version & Project Setup
cmake_minimum_required(VERSION 3.16)
project(TerranEngine LANGUAGES CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# [Basic Setup] ###################################
###################################################

# Collect source files and create engine library
set(ENGINE_SRC
    src/engine/Engine.cpp
    src/engine/Window.cpp
    src/engine/Renderer.cpp
)

# Collect external subdirectories
add_subdirectory(external/GLAD)
# ...

###################################################
# [Library Configuration] #########################
# Create the engine library
add_library(engine STATIC ${ENGINE_SRC})

target_link_libraries(engine
    PRIVATE
        glad
)

# Include directories for the engine
target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external/GLAD/include
        # ...
)
# [Library Configuration] #########################
###################################################


###################################################
# [Executable Configuration] ######################
# Create the executable
add_executable(TerranEngine src/main.cpp)

# Link the engine library to the executable
target_link_libraries(TerranEngine PRIVATE engine)

# Include SDL headers
target_include_directories(TerranEngine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL/include
        # ...
)

# Link the SDL3 library
target_link_libraries(TerranEngine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/SDL3.dll
)

# Specify output directory for the executable
set_target_properties(TerranEngine
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
# [Executable Configuration] ######################
###################################################

# Print summary
message(STATUS "CMake generator: ${CMAKE_GENERATOR}")
message(STATUS "Build type:      ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler:    ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ flags:       ${CMAKE_CXX_FLAGS}")
