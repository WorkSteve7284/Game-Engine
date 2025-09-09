macro(add_modules PROJECT)

    set(CORE_MODULES import/PotatoEngine/Core)

    target_sources(${PROJECT}
        PUBLIC
            FILE_SET cxx_modules TYPE CXX_MODULES
            FILES
                src/Initialize.cppm
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
                            
     )
endmacro()
