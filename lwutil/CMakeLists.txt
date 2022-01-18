cmake_minimum_required(VERSION 3.22)

# Debug message
message("Entering ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")

# Register core library
add_library(lwutil INTERFACE)
target_sources(lwutil PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/lwutil/lwutil.c)
target_include_directories(lwutil INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/include)

# Register other modules

# Debug message
message("Exiting ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")
