cmake_minimum_required(VERSION 3.17)
project(LifProjet)

set(CMAKE_CXX_STANDARD 17)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "lib/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
add_executable(LifProjet src/main.cpp src/Application.cpp src/Application.hpp)
target_link_libraries(LifProjet sfml-graphics sfml-audio)