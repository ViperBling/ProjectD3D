cmake_minimum_required (VERSION 3.1)
project (D3D11)

set (CMAKE_C_STANDARD 11)
set (CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS -EHsc)

include_directories(include)
include_directories(src)

#include (CTest)
add_subdirectory(src)