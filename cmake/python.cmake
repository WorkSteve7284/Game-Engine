macro(run_python FILES)

    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    execute_process(COMMAND ${Python3_EXECUTABLE} cmake/auto_escape.py ${FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE PY_OUTPUT
    )

    message(STATUS ${PY_OUTPUT})
    
endmacro()