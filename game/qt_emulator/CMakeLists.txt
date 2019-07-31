cmake_minimum_required(VERSION 3.1.0)

project(gamebox)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)

# Find the QtWidgets library
find_package(Qt5Widgets)

add_executable(gamebox main.cpp window.cpp renderarea.cpp)
target_sources(gamebox PUBLIC ../BackspaceInvaders.cpp ../BreakOut.cpp ../Flappy.cpp
               ../Raycaster.cpp ../Saper.cpp ../Snake.cpp ../Snail.cpp ../Snake2.cpp
               ../Tester.cpp ../Mario.cpp ../Ghostbuster.cpp ../SpaceShips.cpp
               ../LodeRunner.cpp ../TicTacToe.cpp ../NY2019.cpp ../Tetrisnake.cpp
               ../DoodleJump.cpp ../ColorDemo.cpp ../SpriteDemo.cpp)

target_sources(gamebox PUBLIC ../application.cpp ../avrfix.cpp ../menu.cpp
                         ../random.cpp)

target_sources(gamebox PUBLIC ../application.cpp ../avrfix.cpp
			   ../menu.cpp ../random.cpp)
target_compile_definitions(gamebox PUBLIC EMULATED)

# Use the Widgets module from Qt 5.
target_link_libraries(gamebox Qt5::Widgets)
