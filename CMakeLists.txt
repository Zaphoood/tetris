cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
    src/file.cpp
    src/timer.cpp
)

target_link_libraries(tetris PUBLIC SDL2 SDL2_ttf)
target_include_directories(tetris PRIVATE
    src
    include
    /usr/local/include/SDL2
    /usr/include/SDL2
)

target_compile_definitions(tetris
  PRIVATE
  $<$<CONFIG:Release>: RELEASE>
  $<$<CONFIG:MinSizeRel>: RELEASE>
  $<$<CONFIG:RelWithDebInfo>: RELEASE>
  )

install(FILES assets/futura-medium.ttf DESTINATION $ENV{HOME}/.fonts)
install(TARGETS tetris DESTINATION bin)
