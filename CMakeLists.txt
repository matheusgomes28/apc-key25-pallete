cmake_minimum_required(VERSION 3.20)
project(APC25_PALLETE LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

add_subdirectory(apc25_pallete)
add_subdirectory(apc25_editor)

if(BUILD_TESTING)
  message(STATUS "!!! Building Tests")
endif()
