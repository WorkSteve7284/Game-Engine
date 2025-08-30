macro(add_modules PROJECT)

    set(CORE_MODULES import/PotatoEngine/Core)

    target_sources(${PROJECT}
        PUBLIC
            FILE_SET CXX_MODULES
            FILES
                src/Initialize.cppm
    )

    # Core
    target_sources(${PROJECT}
        PUBLIC
            FILE_SET CXX_MODULES
            FILES
                ${CORE_MODULES}/Core.cppm
                ${CORE_MODULES}/Using.cppm
                # Classes
                ${CORE_MODULES}/Classes/Classes.cppm
                ${CORE_MODULES}/Classes/Component.cppm
                ${CORE_MODULES}/Classes/Object.cppm
                ${CORE_MODULES}/Classes/Manager.cppm
                ${CORE_MODULES}/Classes/Get_Manager.cppm
                # Exception
                ${CORE_MODULES}/Exception/Exception.cppm
                ${CORE_MODULES}/Exception/No_Component_Found.cppm
                ${CORE_MODULES}/Exception/No_Object_Found.cppm
                ${CORE_MODULES}/Exception/No_File_Found.cppm
                # Debug
                ${CORE_MODULES}/Debug/Debug.cppm
                # Time
                ${CORE_MODULES}/Time/Time.cppm
                # Events
                ${CORE_MODULES}/Event/Event.cppm
     )

     set(PHYSICS_MODULES import/PotatoEngine/Physics)

     # Physics
     target_sources(${PROJECT}
        PUBLIC
            FILE_SET CXX_MODULES
            FILES
                ${PHYSICS_MODULES}/Physics.cppm
                # Structs
                ${PHYSICS_MODULES}/Structs/Structs.cppm
                # Vector
                ${PHYSICS_MODULES}/Structs/Vector/Vector.cppm
                ${PHYSICS_MODULES}/Structs/Vector/Vector2.cppm
                ${PHYSICS_MODULES}/Structs/Vector/Vector3.cppm
                # Quaternion
                ${PHYSICS_MODULES}/Structs/Quaternion/Quaternion.cppm
                # Matrix
                ${PHYSICS_MODULES}/Structs/Matrix/Matrix.cppm
                ${PHYSICS_MODULES}/Structs/Matrix/Matrix3x3.cppm
                
                # Exception
                ${PHYSICS_MODULES}/Exception/Exception.cppm
                ${PHYSICS_MODULES}/Exception/Non_Invertible_Matrix.cppm
                
     )
endmacro()
