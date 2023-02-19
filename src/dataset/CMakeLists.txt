set(CURRENT_INCLUDE_DIR ${INCLUDE_DIR}/nn)

add_library(neuralNetwork STATIC
        dataset_manager.c ${CURRENT_INCLUDE_DIR}/dataset_manager.h
 
        ${INCLUDE_DIR}/type.h
        ${INCLUDE_DIR}/global.h
        )

set_target_properties(neuralNetwork PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

target_include_directories(neuralNetwork PUBLIC "${CURRENT_INCLUDE_DIR}" "${INCLUDE_DIR}" )



