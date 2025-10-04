function(add_modules PROJECT)
    
    # Convert YAML object prefabs into header files
    if(DEFINED RELEASE)
        file(GLOB_RECURSE PREFABS "${CMAKE_SOURCE_DIR}/src/prefabs/*.yaml")
    
        include(${CMAKE_SOURCE_DIR}/cmake/python.cmake)
        foreach(PREFAB ${PREFABS})
            run_python(${PREFAB})
        endforeach()
    endif()
    
    set(CORE_MODULES ${CMAKE_SOURCE_DIR}/import/PotatoEngine/Core)

    target_sources(${PROJECT}
        PUBLIC
            FILE_SET cxx_modules TYPE CXX_MODULES
            FILES
                ${CMAKE_SOURCE_DIR}/src/Initialize.cppm
                ${CMAKE_SOURCE_DIR}/src/Components.cppm
    )

    # Core
    target_sources(${PROJECT}
        PUBLIC
            FILE_SET cxx_modules TYPE CXX_MODULES
            FILES
                ${CORE_MODULES}/Core.cppm
                ${CORE_MODULES}/Using.cppm
                # Classes
                ${CORE_MODULES}/Classes/Classes.cppm
                ${CORE_MODULES}/Classes/Component.cppm
                ${CORE_MODULES}/Classes/Object.cppm
                ${CORE_MODULES}/Classes/Manager.cppm
                ${CORE_MODULES}/Classes/SafePtr.cppm
                ${CORE_MODULES}/Classes/ObjectMap.cppm
                # Exception
                ${CORE_MODULES}/Exception/Exception.cppm
                ${CORE_MODULES}/Exception/NoComponentFound.cppm
                ${CORE_MODULES}/Exception/NoObjectFound.cppm
                ${CORE_MODULES}/Exception/NoFileFound.cppm
                ${CORE_MODULES}/Exception/InvalidPointer.cppm
                # Debug
                ${CORE_MODULES}/Debug/Debug.cppm
                # Time
                ${CORE_MODULES}/Time/Time.cppm
                ${CORE_MODULES}/Time/TimeVariables.cppm
                ${CORE_MODULES}/Time/TimeKeeper.cppm
                # Events
                ${CORE_MODULES}/Event/Event.cppm
                # Initialization
                ${CORE_MODULES}/Init/Init.cppm
                ${CORE_MODULES}/Init/File.cppm
                ${CORE_MODULES}/Init/YAML.cppm
                ${CORE_MODULES}/Init/ParseYAML.cppm
                ${CORE_MODULES}/Init/Prefab.cppm
                ${CORE_MODULES}/Init/ParseObject.cppm
                ${CORE_MODULES}/Init/CreateObject.cppm
    )
endfunction()
