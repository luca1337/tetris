cmake_minimum_required(VERSION 3.18)

find_package(SDL2 CONFIG REQUIRED)

add_executable(tetris 
    "src/main.cpp"
    "src/Texture.cpp"
    "src/Window.cpp"
    "src/Board.cpp"
    "src/Input.cpp"
    "src/Block.cpp"
    "src/tetraminos/Tetramino.cpp"

    "src/fsm/base/FSM.cpp"
    "src/fsm/board/StatePlay.cpp"

    "src/game/Game.cpp"
    "include/Window.h"
    "include/Texture.h"
    "include/Board.h"
    "include/stb_image.h"
    "include/Constants.h"
    "include/Types.h"
    "include/Utils.h"

    "include/fsm/base/FSM.h"
    "include/fsm/base/IState.h"
    "include/fsm/board/StatePlay.h"

    "include/game/Game.h"
    "include/Input.h"
    "include/rng.h"
    "include/Block.h"
    "include/tetraminos/Tetramino.h"
)

target_compile_features(tetris PRIVATE cxx_std_20)

target_link_libraries(tetris PRIVATE SDL2::SDL2main SDL2::SDL2)