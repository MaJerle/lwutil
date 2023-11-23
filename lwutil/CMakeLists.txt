cmake_minimum_required(VERSION 3.22)

# Register core library
add_library(lwutil INTERFACE)
target_sources(lwutil PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/lwutil/lwutil.c)
target_include_directories(lwutil INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/include)

# Register other modules
