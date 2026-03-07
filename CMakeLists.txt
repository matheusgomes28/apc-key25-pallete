cmake_minimum_required(VERSION 3.20)
project(APC25_PALLETE LANGUAGES CXX)

add_subdirectory(apc25_pallete)

if(BUILD_TESTING)
  message(STATUS "!!! Building Tests")
endif()
