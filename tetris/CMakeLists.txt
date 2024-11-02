cmake_minimum_required(VERSION 3.18)

project(tetris VERSION 0.1.0 LANGUAGES C CXX)

find_package(glm CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

set(CMAKE_VERBOSE_MAKEFILE ON)

# Trova tutti i file .cpp e .h nelle directory dei sorgenti e degli include
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp ${INCLUDE_DIR}/*.h)

# Aggiungi l'eseguibile
add_executable(tetris ${SOURCES})

# Includi le directory degli header
target_include_directories(tetris PRIVATE ${INCLUDE_DIR})

target_compile_features(tetris PRIVATE cxx_std_23)

target_link_libraries(tetris PRIVATE 
    glm::glm 
    SDL2::SDL2 
    SDL2_image::SDL2_image 
    SDL2_ttf::SDL2_ttf
    glad::glad
    spdlog::spdlog
    SDL2_mixer::SDL2_mixer
)

target_compile_definitions(tetris PUBLIC GLM_ENABLE_EXPERIMENTAL)