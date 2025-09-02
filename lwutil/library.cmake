# 
# LIB_PREFIX: LWUTIL
#
# This file provides set of variables for end user
# and also generates one (or more) libraries, that can be added to the project using target_link_libraries(...)
#
# Before this file is included to the root CMakeLists file (using include() function), user can set some variables:
#
# LWUTIL_COMPILE_OPTIONS: If defined, it provide compiler options for generated library.
# LWUTIL_COMPILE_DEFINITIONS: If defined, it provides "-D" definitions to the library build
#

# Custom include directory
set(LWUTIL_CUSTOM_INC_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib_inc)

# Library core sources
set(lwutil_core_SRCS 
    ${CMAKE_CURRENT_LIST_DIR}/src/lwutil/lwutil.c
)

# Setup include directories
set(lwutil_include_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/src/include
    ${LWUTIL_CUSTOM_INC_DIR}
)

# Register library to the system
add_library(lwutil)
target_sources(lwutil PRIVATE ${lwutil_core_SRCS})
target_include_directories(lwutil PUBLIC ${lwutil_include_DIRS})
target_compile_options(lwutil PRIVATE ${LWUTIL_COMPILE_OPTIONS})
target_compile_definitions(lwutil PRIVATE ${LWUTIL_COMPILE_DEFINITIONS})
