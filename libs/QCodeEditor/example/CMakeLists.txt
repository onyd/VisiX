cmake_minimum_required(VERSION 3.6)
project(QCodeEditorExample)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC On)
set(CMAKE_AUTORCC ON)

find_package(${QT_VERSION_MAJOR}Core    CONFIG REQUIRED)
find_package(${QT_VERSION_MAJOR}Widgets CONFIG REQUIRED)
find_package(${QT_VERSION_MAJOR}Gui     CONFIG REQUIRED)

add_executable(QCodeEditorExample
    resources/demo_resources.qrc
    src/main.cpp
    src/MainWindow.cpp
    include/MainWindow.hpp
)

target_include_directories(QCodeEditorExample PUBLIC
    include
)

target_link_libraries(QCodeEditorExample
    ${QT_VERSION_MAJOR}::Core
    ${QT_VERSION_MAJOR}::Widgets
    ${QT_VERSION_MAJOR}::Gui
    QCodeEditor
)