cmake_minimum_required(VERSION 3.16)

project(qtglconfig LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(qtglconfig SHARED
  src/qtglconfig_global.h
  src/qtglconfig.cpp
  src/qtglconfig.h
)

target_link_libraries(qtglconfig PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(qtglconfig PRIVATE QTGLCONFIG_LIBRARY)


include("./output/makeme.cmake")
