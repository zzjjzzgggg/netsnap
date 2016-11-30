# -*- mode: cmake;-*-

cmake_minimum_required(VERSION 3.0.0)
project (GC C CXX)

# compiler
set(CMAKE_CXX_COMPILER=/usr/bin/g++)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++11 -pthread")

# output directories
set(CMAKE_BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_BINARY_DIR ${CMAKE_BUILD_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BUILD_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# required libraries from system
find_package(Threads REQUIRED)
find_package(gflags REQUIRED)

# common libraries that are required by every modular
set(LIBDIR /home/jzzhao/git_project)
set(LIBSNAP ${LIBDIR}/netsnap)
set(LIBCPP ${LIBDIR}/cpplib)
set(LIBASSERT ${LIBCPP}/Assert/src)

include_directories(${LIBCPP} ${LIBASSERT} ${LIBSNAP})
include_directories(${GC_SOURCE_DIR}/src)

add_library(snap SHARED ${LIBSNAP}/snap/Snap.cpp)
add_library(ppk_assert SHARED ${LIBASSERT}/ppk_assert.cpp)
add_library(stringutils SHARED ${LIBCPP}/stringutils.cpp)
add_library(argsparser SHARED ${LIBCPP}/argsparser.cpp)
target_link_libraries(argsparser stringutils)

set(BASE_LIBS ${BASE_LIBS} ppk_assert gflags stringutils
  argsparser snap)

add_subdirectory(src)
