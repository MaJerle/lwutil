# Library core sources
set(lwutil_core_SRCS 
    ${CMAKE_CURRENT_LIST_DIR}/src/lwutil/lwutil.c
)

# Setup include directories
set(lwutil_include_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/src/include
)

# Register library to the system
add_library(lwutil INTERFACE)
target_sources(lwutil INTERFACE ${lwutil_core_SRCS})
target_include_directories(lwutil INTERFACE ${lwutil_include_DIRS})
target_compile_options(lwutil PRIVATE ${LWUTIL_COMPILE_OPTIONS})
target_compile_definitions(lwutil PRIVATE ${LWUTIL_COMPILE_DEFINITIONS})
