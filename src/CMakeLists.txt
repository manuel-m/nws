cmake_minimum_required(VERSION 3.0)

project (nws)

set (ET_BINNAME "nws")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set (ET_BINNAME "${ET_BINNAME}_d")
endif (CMAKE_BUILD_TYPE MATCHES Debug)

add_executable(${ET_BINNAME} main.cpp)
