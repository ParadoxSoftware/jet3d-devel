cmake_minimum_required(VERSION 3.13)

set(DEPENDENCIES_PATH ${CMAKE_CURRENT_LIST_DIR}/dependencies)

project(Jet3D C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(source)
