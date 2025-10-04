cmake_minimum_required(VERSION 3.30)

project(PotatoEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD ON)

add_subdirectory(engine)
add_subdirectory(game)