macro(add_modules PROJECT)

    set(CORE_MODULE_DIR import/PotatoEngine/Core)

    target_sources(${PROJECT}
        PUBLIC
            FILE_SET CXX_MODULES
            FILES
                import/PotatoEngine/PotatoEngine.cppm
                src/Initialize.cppm
                ${CORE_MODULE_DIR}/Core.cppm
                ${CORE_MODULE_DIR}/Using.cppm
                # Classes
                ${CORE_MODULE_DIR}/Classes/Classes.cppm
                ${CORE_MODULE_DIR}/Classes/Component.cppm
                ${CORE_MODULE_DIR}/Classes/Object.cppm
                ${CORE_MODULE_DIR}/Classes/Manager.cppm
                ${CORE_MODULE_DIR}/Classes/Get_Manager.cppm
                # Exception
                ${CORE_MODULE_DIR}/Exception/Exception.cppm
                ${CORE_MODULE_DIR}/Exception/No_Component_Found.cppm
                ${CORE_MODULE_DIR}/Exception/No_Object_Found.cppm
                ${CORE_MODULE_DIR}/Exception/No_File_Found.cppm
                # Debug
                ${CORE_MODULE_DIR}/Debug/Debug.cppm
                # Time
                ${CORE_MODULE_DIR}/Time/Time.cppm
                # Events
                ${CORE_MODULE_DIR}/Event/Event.cppm

        PRIVATE
            # Implementation Files
     )

endmacro()
