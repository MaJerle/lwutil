cmake_minimum_required(VERSION 3.22)

# Setup project
project(LwLibPROJECT)

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

enable_testing()
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test_main.c
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/dev
)

# Include file that can add more sources and prepare lib parameters
include(${TEST_CMAKE_FILE_NAME})

# Add subdir with lwutil and link to the project
add_subdirectory("../lwutil" lwutil)
target_link_libraries(${PROJECT_NAME} PUBLIC lwutil)

# Add additional compile settings to library, which will be propagated to executable (PUBLIC link)
target_compile_options(lwutil PUBLIC -Wall -Wextra -Wpedantic)
target_compile_definitions(lwutil PUBLIC WIN32 _DEBUG CONSOLE LWUTIL_DEV)

# Add test
add_test(NAME Test COMMAND $<TARGET_FILE:${CMAKE_PROJECT_NAME}>)