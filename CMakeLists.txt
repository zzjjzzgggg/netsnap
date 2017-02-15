cmake_minimum_required(VERSION 3.5.1)
project (netsnap CXX)

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

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

include_directories(glib)
include_directories(snap)

add_subdirectory(glib)
add_subdirectory(snap)
add_subdirectory(test)
