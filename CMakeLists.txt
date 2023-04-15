cmake_minimum_required(VERSION 3.25)
project(game_of_life)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(game_of_life
        main.cpp
        src/DrawableTableWidget.cpp
        src/MainWindow.cpp
        )
target_include_directories(game_of_life PRIVATE src)
target_link_libraries(game_of_life PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
