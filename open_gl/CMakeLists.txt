cmake_minimum_required(VERSION 3.16)
project(open_gl)

set(CMAKE_CXX_STANDARD 17)

add_executable(open_gl main.cpp)

find_package(glfw3 REQUIRED)

include_directories(${glfw3_INCLUDE_DIRS})
target_link_libraries(open_gl ${glfw3_LIBRARY})