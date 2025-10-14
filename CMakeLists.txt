cmake_minimum_required(VERSION 3.30)

project(PotatoEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD ON)

add_subdirectory(external/glfw ${CMAKE_SOURCE_DIR}/external/glfw/build)
add_subdirectory(external/glad ${CMAKE_SOURCE_DIR}/external/glad/build)
add_subdirectory(engine ${CMAKE_SOURCE_DIR}/engine/build)

# Compile specified game
set(GAME "" CACHE STRING "Which game to build")

if(GAME)
    message(STATUS "Compiling ${GAME}")
    add_subdirectory(games/${GAME} ${CMAKE_SOURCE_DIR}/games/${GAME}/build)
else()
    message(WARNING "No game specified to compile! Only compiling engine code.")
endif()