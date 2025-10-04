cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")

project(PE-Test LANGUAGES CXX)

#set(CMAKE_CXX_MODULE_STD 1)

add_executable(PE-Test 
    src/PotatoEngine.cpp
 )
target_compile_features(PE-Test PUBLIC cxx_std_23)

set_target_properties(PE-Test PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

include(cmake/PotatoEngine.cmake)
include(cmake/Setup.cmake)

setup(PE-Test)

add_modules(PE-Test)

target_sources(PE-Test
        PUBLIC
            FILE_SET CXX_MODULES
            FILES
            src/test.cppm
)

message(STATUS "MSVC Version: ${MSVC_VERSION}")
message(STATUS "CMake Version: ${CMAKE_VERSION}")