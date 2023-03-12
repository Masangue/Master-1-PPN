set(CURRENT_INCLUDE_DIR ${INCLUDE_DIR}/convolution_neural_network/neural_network)
set(CT_INCLUDE_DIR ${INCLUDE_DIR}/context)

add_library(neural_network STATIC
        neural_network.c ${CURRENT_INCLUDE_DIR}/neural_network.h
        activation.c ${CURRENT_INCLUDE_DIR}/activation.h
 
        ${CT_INCLUDE_DIR}/context.h

        ${INCLUDE_DIR}/type.h
        ${INCLUDE_DIR}/global.h
        )

set_target_properties(neural_network PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

target_link_libraries(neural_network PRIVATE context )


target_include_directories(neural_network PUBLIC 
    "${CURRENT_INCLUDE_DIR}" "${INCLUDE_DIR}" "${CT_INCLUDE_DIR}" )



