cmake_minimum_required(VERSION 3.31)

set(DEPENDENCIES_PATH ${CMAKE_CURRENT_LIST_DIR}/dependencies)
set(JET_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

project(Jet3D CXX ASM_MASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(dependencies)
add_subdirectory(source)
