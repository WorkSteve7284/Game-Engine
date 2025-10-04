# --- Main library --- #

add_library(PotatoEngine INTERFACE)

# --- Core Library --- #

add_library(PotatoEngine-Core STATIC)

# Add modules
target_sources(PotatoEngine-Core
    PUBLIC
    FILE_SET CXX_MODULES FILES

        import/Core/Core.cppm
        import/Core/Using.cppm

        import/Core/Classes/Classes.cppm
        import/Core/Classes/Component.cppm
        import/Core/Classes/Object.cppm
        import/Core/Classes/Manager.cppm
        import/Core/Classes/ObjectMap.cppm
        import/Core/Classes/SafePtr.cppm

        import/Core/Debug/Debug.cppm
        
        import/Core/Event/Event.cppm

        import/Core/Exception/Exception.cppm
        import/Core/Exception/InvalidPointer.cppm
        import/Core/Exception/NoComponentFound.cppm
        import/Core/Exception/NoObjectFound.cppm
        import/Core/Exception/NoFileFound.cppm

        import/Core/Init/Init.cppm
        import/Core/Init/File.cppm
        import/Core/Init/YAML.cppm
        import/Core/Init/Prefab.cppm
        import/Core/Init/ParseYaml.cppm
        import/Core/Init/ParseObject.cppm

        import/Core/Time/Time.cppm
        import/Core/Time/TimeKeeper.cppm
        import/Core/Time/TimeVariables.cppm
)

target_link_libraries(PotatoEngine INTERFACE PotatoEngine-Core)