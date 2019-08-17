cmake_minimum_required(VERSION 3.1.0)

project(SpriteEditor)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)

# Find the QtWidgets library
find_package(Qt5Widgets)

add_executable(SpriteEditor main.cpp window.cpp sprite.cpp palette.cpp)

# Use the Widgets module from Qt 5.
target_link_libraries(SpriteEditor Qt5::Widgets)
