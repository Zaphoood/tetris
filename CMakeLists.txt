cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

# set the project name
project(Tetris)

add_executable(tetris
    src/main.cpp
    src/active.cpp
    src/bag.cpp
    src/game.cpp
    src/hud.cpp
    src/main.cpp
    src/playfield.cpp
    src/scoring.cpp
    src/tetrovis.cpp
)


target_link_libraries(tetris PUBLIC SDL2 SDL2_ttf)
target_include_directories(tetris PRIVATE
    src
    include
    /usr/local/include/SDL2
)
