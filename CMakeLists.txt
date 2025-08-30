cmake_minimum_required(VERSION 3.28)

project(PE-Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(PE-Test 
    src/PotatoEngine.cpp
)

include(cmake/PotatoEngine.cmake)
include(cmake/Setup.cmake)

setup(PE-Test)

add_modules(PE-Test)

target_sources(PE-Test
        PUBLIC
            FILE_SET CXX_MODULES
            FILES
                src/Stop_After_Time.cppm
                src/Event_Reciever.cppm
                src/Event_Broadcaster.cppm
                src/Move_At_Speed.cppm
)