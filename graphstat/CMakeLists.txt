# -*- mode: cmake;-*-

cmake_minimum_required(VERSION 3.0.0)
project (GRAPH_STAT C CXX)

include(CheckCXXCompilerFlag)

check_cxx_compiler_flag("-std=c++1z" COMPILER_SUPPORTS_CXX_1z)
check_cxx_compiler_flag("-std=c++14" COMPILER_SUPPORTS_CXX_14)
check_cxx_compiler_flag("-std=c++17" COMPILER_SUPPORTS_CXX_17)

if (COMPILER_SUPPORTS_CXX_1z)
  message(STATUS "Compiling with C++1z")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
elseif (COMPILER_SUPPORTS_CXX_14)
  message(STATUS "Compiling with C++14")
  set(CMAKE_CXX_STANDARD 14)
else()
  message(FATAL_ERROR "Please install a modern C++ compiler.")
endif()

# output directories
set(CMAKE_BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_BINARY_DIR ${CMAKE_BUILD_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BUILD_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BUILD_DIR})

# cmake package search directories
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{GIT_REPO})

# required libraries from system
find_package(Threads REQUIRED)

# gflags
find_package(gflags REQUIRED)

# cpplib
find_package(CPPLIB REQUIRED)
add_definitions(${CPPLIB_DEFINITIONS})
include_directories(${CPPLIB_INCLUDE_DIRS})

# netsnap
find_package(NETSNAP REQUIRED)
add_definitions(${NETSNAP_DEFINITIONS})
include_directories(${NETSNAP_INCLUDE_DIRS})

set(LIBRARIES ${LIBRARIES} gflags ${CPPLIB_LIBRARIES} ${NETSNAP_LIBRARIES})

add_subdirectory(src)
