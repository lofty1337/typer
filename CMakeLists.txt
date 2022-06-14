cmake_minimum_required(VERSION 3.20)
project(typer)

set(CMAKE_CXX_STANDARD 14)

add_executable(typer main.cpp inc/header.h classes.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(SFML/include)
include_directories(inc/)
target_link_libraries(typer sfml-system sfml-window sfml-graphics sfml-audio)

file(COPY "fonts" DESTINATION "/")
file(COPY "texts" DESTINATION "/")