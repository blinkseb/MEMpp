cmake_minimum_required(VERSION 2.6)

# CMAKE_CURRENT_LIST_DIR cmake 2.6 compatibility
if(${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 6)
    get_filename_component(CMAKE_CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
endif(${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 6)

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")

file(GLOB_RECURSE MODULES_SOURCES modules/*.cc)
add_library(modules SHARED ${MODULES_SOURCES})

file(GLOB_RECURSE SOURCES src/*.cc)
add_executable(modules_test ${SOURCES})
target_link_libraries(modules_test dl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O2 -fcx-fortran-rules -fcx-limited-range")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
