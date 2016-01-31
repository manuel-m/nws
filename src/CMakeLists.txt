cmake_minimum_required(VERSION 3.0)

project (nws)
set(etROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${etROOT}/out/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${etROOT}/out/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${etROOT}/out/bin)

set (etD "")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set (etD "${etD}_d")
endif (CMAKE_BUILD_TYPE MATCHES Debug)

add_executable(nws${etD} main.cpp)


add_subdirectory(tests)