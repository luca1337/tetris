cmake_minimum_required(VERSION 3.18)
project(tetris VERSION 0.1.0 LANGUAGES C CXX)

include_directories(${CMAKE_SOURCE_DIR}/tetris/include)

add_subdirectory("tetris")