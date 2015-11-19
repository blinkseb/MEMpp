cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules")

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

set(EXTERNAL_DIR "${PROJECT_SOURCE_DIR}/external")
set(EXTERNAL_INCLUDE_DIR "${EXTERNAL_DIR}/include")
set(EXTERNAL_LIB_DIR "${EXTERNAL_DIR}/lib")
set(EXTERNAL_SRC_DIR "${EXTERNAL_DIR}/src")

include_directories(${EXTERNAL_INCLUDE_DIR})

add_subdirectory(external)

include_directories(${EXTERNAL_DIR}/tcl)
include_directories(${EXTERNAL_DIR}/lua)

find_library(CUBA_LIBRARY cuba ${EXTERNAL_LIB_DIR} REQUIRED)
target_link_libraries(modules_test ${CUBA_LIBRARY})
target_link_libraries(modules_test tcl)
target_link_libraries(modules_test lua)

find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})
target_link_libraries(modules ${ROOT_LIBRARIES})
target_link_libraries(modules_test ${ROOT_LIBRARIES})
