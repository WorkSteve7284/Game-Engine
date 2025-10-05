add_executable(game
    PotatoEngine.cpp
    CreateObject.cppm
    Initialize.cppm
    Components.cppm
)

target_sources(game
    PUBLIC
    FILE_SET CXX_MODULES FILES

    src/test.cppm
)

target_link_libraries(game PRIVATE PotatoEngine)

set_target_properties(game PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/game)