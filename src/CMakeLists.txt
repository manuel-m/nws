cmake_minimum_required(VERSION 3.0)

project (nws)
set(etROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${etROOT}/out/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${etROOT}/out/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${etROOT}/out/bin)

set (etD "")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set (etD "${etD}_d")
endif (CMAKE_BUILD_TYPE MATCHES Debug)

add_subdirectory(libs)

if (CMAKE_BUILD_TYPE MATCHES Debug)
  add_subdirectory(tests)
endif (CMAKE_BUILD_TYPE MATCHES Debug)

add_executable(nws${etD} main.cpp)


