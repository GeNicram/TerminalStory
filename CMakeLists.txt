cmake_minimum_required (VERSION 2.6)

project (TerminalStory)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

configure_file (
  "${PROJECT_SOURCE_DIR}/src/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
 
include_directories(${PROJECT_BINARY_DIR})

add_library(Render "src/render.cc")
add_library(Entity "src/entity.cc")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(TerminalStory src/main.cc)
target_link_libraries (TerminalStory Render)
target_link_libraries (TerminalStory Entity)
target_link_libraries (TerminalStory ${CURSES_LIBRARIES})
