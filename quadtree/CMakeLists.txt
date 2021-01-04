cmake_minimum_required(VERSION 3.17)
project(quadtree)

set(CMAKE_CXX_STANDARD 17)

add_executable(quadtree main.cpp point.h Rectangle.h quadtree.h vec.h)