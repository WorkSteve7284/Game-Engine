project(PotatoEngine-Lib LANGUAGES CXX)

# Main Library
add_library(PotatoEngine INTERFACE)

# Add Libraries
add_subdirectory(Core)
add_subdirectory(Rendering)

# Link Libraries
target_link_libraries(PotatoEngine 
    INTERFACE
        PotatoEngine-Core
        PotatoEngine-Rendering
)