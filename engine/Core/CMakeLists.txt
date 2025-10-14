add_library(PotatoEngine-Core STATIC)

target_sources(PotatoEngine-Core
    PUBLIC
    FILE_SET CXX_MODULES FILES

        src/Core.cppm
        src/Using.cppm

        src/Classes/Classes.cppm
        src/Classes/Component.cppm
        src/Classes/Object.cppm
        src/Classes/Manager.cppm
        src/Classes/ObjectMap.cppm
        src/Classes/SafePtr.cppm

        src/Debug/Debug.cppm
        
        src/Event/Event.cppm

        src/Exception/Exception.cppm
        src/Exception/InvalidPointer.cppm
        src/Exception/NoComponentFound.cppm
        src/Exception/NoObjectFound.cppm
        src/Exception/NoFileFound.cppm

        src/Init/Init.cppm
        src/Init/File.cppm
        src/Init/YAML.cppm
        src/Init/Prefab.cppm
        src/Init/ParseYaml.cppm
        src/Init/ParseObject.cppm

        src/Time/Time.cppm
        src/Time/TimeKeeper.cppm
        src/Time/TimeVariables.cppm
)

set_target_properties(PotatoEngine-Core PROPERTIES FOLDER "PotatoEngine")