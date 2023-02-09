set(CURRENT_INCLUDE_DIR ${INCLUDE_DIR})
add_library(SRC STATIC
        file_manager.c ${CURRENT_INCLUDE_DIR}/file_manager.h
        image_processing.c ${CURRENT_INCLUDE_DIR}/image_processing.h
        nn.c ${CURRENT_INCLUDE_DIR}/nn.h
        store.c ${CURRENT_INCLUDE_DIR}/store.h
        evaluation.c ${CURRENT_INCLUDE_DIR}/evaluation.h
        )


target_include_directories(SRC PUBLIC "${CURRENT_INCLUDE_DIR}" )
target_link_libraries(SRC PUBLIC spng)
