cmake_minimum_required(VERSION 3.18)

find_package(glm CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

add_executable(tetris 
    "src/main.cpp"
    "src/Texture.cpp"
    "src/Window.cpp"
    "src/Board.cpp"
    "src/Input.cpp"
    "src/Tetramino.cpp"
    "src/Timer.cpp"

    "src/RenderText.cpp"

    "src/gui/MenuItem.cpp"
    "src/gui/MenuSettingsPage.cpp"

    "src/fsm/base/FSM.cpp"
    "src/fsm/board/StatePlay.cpp"
    "src/fsm/board/StateCheck.cpp"
    "src/fsm/board/StateBlink.cpp"
    "src/fsm/board/StateClear.cpp"
    "src/fsm/game/StateSetup.cpp"
    "src/fsm/game/StateMainMenu.cpp"

    "src/game/Game.cpp"
    "include/Window.h"
    "include/Texture.h"
    "include/Board.h"
    "include/stb_image.h"
    "include/Constants.h"
    "include/Types.h"
    "include/Utils.h"

    "include/gui/MenuItem.h"
    "include/gui/MenuSettingsPage.h"

    "include/fsm/base/FSM.h"
    "include/fsm/base/IState.h"
    "include/fsm/board/StatePlay.h"
    "include/fsm/board/StateCheck.h"
    "include/fsm/board/StateBlink.h"
    "include/fsm/board/StateClear.h"
    "include/fsm/game/StateSetup.h"
    "include/fsm/game/StateMainMenu.h"

    "include/RenderText.h"

    "include/game/Game.h"
    "include/Input.h"
    "include/Random.h"
    "include/Tetramino.h"
    "include/Timer.h"
)

target_compile_features(tetris PRIVATE cxx_std_23)

target_link_libraries(tetris PRIVATE glm::glm)
target_link_libraries(tetris PRIVATE SDL2::SDL2main SDL2::SDL2)
target_link_libraries(tetris PRIVATE SDL2_image::SDL2_image)
target_link_libraries(tetris PRIVATE SDL2_ttf::SDL2_ttf)