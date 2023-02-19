set(CURRENT_INCLUDE_DIR ${INCLUDE_DIR}/convolution_neural_network/convolution_layer)

add_library(convolution_layer STATIC
        convolution_layer.c ${CURRENT_INCLUDE_DIR}/convolution_layer.h
 
        ${INCLUDE_DIR}/type.h
        ${INCLUDE_DIR}/global.h
        )

set_target_properties(convolution_layer PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

target_include_directories(convolution_layer PUBLIC "${CURRENT_INCLUDE_DIR}" "${INCLUDE_DIR}" )



