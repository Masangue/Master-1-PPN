set(CURRENT_INCLUDE_DIR ${INCLUDE_DIR}/image)
set(CT_INCLUDE_DIR ${INCLUDE_DIR}/context)

add_library(image STATIC

        dataset_manager.c ${CURRENT_INCLUDE_DIR}/dataset_manager.h
        image_io.c ${CURRENT_INCLUDE_DIR}/image_io.h

        ${INCLUDE_DIR}/type.h
        ${INCLUDE_DIR}/global.h
        )
set_target_properties(image PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

target_include_directories(image PUBLIC "${CURRENT_INCLUDE_DIR}"
                "${INCLUDE_DIR}" "${CT_INCLUDE_DIR}" )

target_link_libraries(image PUBLIC spng convolution_layer context )



