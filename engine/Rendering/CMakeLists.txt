add_library(PotatoEngine-Rendering STATIC)

target_sources(PotatoEngine-Rendering 
    PUBLIC
    FILE_SET CXX_MODULES FILES
        src/Rendering.cppm

        src/Window/Window.cppm

        src/Exception/Exception.cppm
        src/Exception/WindowError.cppm

)

target_link_libraries(PotatoEngine-Rendering PRIVATE PotatoEngine-Core)
target_link_libraries(PotatoEngine-Rendering PRIVATE glfw)
target_link_libraries(PotatoEngine-Rendering PRIVATE glad)

set_target_properties(PotatoEngine-Rendering PROPERTIES FOLDER "PotatoEngine")