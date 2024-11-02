cmake_minimum_required(VERSION 3.18)
project(tetris VERSION 0.1.0 LANGUAGES C CXX)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/tetris/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/tetris/include)

add_subdirectory("tetris")

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)