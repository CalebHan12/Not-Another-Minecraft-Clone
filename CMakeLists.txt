cmake_minimum_required(VERSION 3.14...4.0)

include(FetchContent)

# TODO: any configs to each submodule to reduce footprint
# TODO: copy textures and shaders to build dir

set(project_name NondescriptVoxelGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

set(GLFW_BUILD_WAYLAND OFF)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_Declare(
    catch
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.11.0
)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG 1.0.2
)

FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(catch)
FetchContent_MakeAvailable(glm)

project(
        ${project_name}
        VERSION 1.0
        LANGUAGES C CXX
)

set(src_files
    ${CMAKE_SOURCE_DIR}/src/Blocks.cpp
    ${CMAKE_SOURCE_DIR}/src/Camera.cpp
    ${CMAKE_SOURCE_DIR}/src/Chunk.cpp
    ${CMAKE_SOURCE_DIR}/src/ChunkMesh.cpp
    ${CMAKE_SOURCE_DIR}/src/Game.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/Shader.cpp
    ${CMAKE_SOURCE_DIR}/src/Texture.cpp
    ${CMAKE_SOURCE_DIR}/src/glad.c
    ${CMAKE_SOURCE_DIR}/src/WorldManager.cpp
    ${CMAKE_SOURCE_DIR}/src/Player.cpp
)

set(header_files
    ${CMAKE_SOURCE_DIR}/src/Blocks.h
    ${CMAKE_SOURCE_DIR}/src/Camera.h
    ${CMAKE_SOURCE_DIR}/src/Chunk.h
    ${CMAKE_SOURCE_DIR}/src/ChunkMesh.h
    ${CMAKE_SOURCE_DIR}/src/Game.h
    ${CMAKE_SOURCE_DIR}/src/Shader.h
    ${CMAKE_SOURCE_DIR}/src/Texture.h
    ${CMAKE_SOURCE_DIR}/src/WorldManager.h
    ${CMAKE_SOURCE_DIR}/src/Player.h
)

add_executable(
    ${project_name}
    ${src_files}
    ${header_files}
)

file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/src/textures DESTINATION ${CMAKE_BINARY_DIR})

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message("We are using GNU")
    target_compile_options(${project_name} PRIVATE -Wall -Wextra -pedantic -Wno-psabi -g -O0)
    #target_compile_options(${project_name} PRIVATE -Wall -Wextra -pedantic -Wno-psabi -O3)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${project_name} PRIVATE -Weverything -Wno-c++98-compat)
endif()

target_include_directories(${project_name}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/ext/
)

target_link_libraries(${project_name}
        PRIVATE
        Catch2::Catch2
        glfw
        glm
)
