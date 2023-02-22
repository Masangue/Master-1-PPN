set(CURRENT_INCLUDE_DIR ${INCLUDE_DIR}/convolution_neural_network)
set(NN_INCLUDE_DIR ${INCLUDE_DIR}/convolution_neural_network/neural_network)
set(CL_INCLUDE_DIR ${INCLUDE_DIR}/convolution_neural_network/convolution_layer)
set(IMAGE_INCLUDE_DIR ${INCLUDE_DIR}/image)

add_library(convolution_neural_network STATIC
        store.c ${CURRENT_INCLUDE_DIR}/store.h
        evaluation.c ${CURRENT_INCLUDE_DIR}/evaluation.h
        training.c ${CURRENT_INCLUDE_DIR}/training.h
 
        ${NN_INCLUDE_DIR}/neural_network.h
        ${IMAGE_INCLUDE_DIR}/dataset_manager.h
        ${CL_INCLUDE_DIR}/convolution_layer.h

        ${INCLUDE_DIR}/type.h
        ${INCLUDE_DIR}/global.h
        )

set_target_properties(convolution_neural_network  PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

target_include_directories(convolution_neural_network PUBLIC 
            "${CURRENT_INCLUDE_DIR}" "${INCLUDE_DIR}" 
            "${NN_INCLUDE_DIR}" "${IMAGE_INCLUDE_DIR}"
            "${CL_INCLUDE_DIR}" )

target_link_libraries(convolution_neural_network PRIVATE 
            neural_network image )

add_subdirectory(convolution_layer)
add_subdirectory(neural_network)
