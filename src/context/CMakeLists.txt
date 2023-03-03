set(CURRENT_INCLUDE_DIR ${INCLUDE_DIR}/context)
set(NN_INCLUDE_DIR ${INCLUDE_DIR}/convolution_neural_network/neural_network)

add_library(context STATIC

        context.c ${CURRENT_INCLUDE_DIR}/context.h
        ${NN_INCLUDE_DIR}/neural_network.h

        ${INCLUDE_DIR}/type.h
        ${INCLUDE_DIR}/global.h
        )
set_target_properties(context PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

target_include_directories(context PUBLIC "${CURRENT_INCLUDE_DIR}" "${INCLUDE_DIR}" )

target_link_libraries(context PUBLIC config )



