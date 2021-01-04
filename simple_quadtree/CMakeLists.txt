cmake_minimum_required(VERSION 3.17)
project(simple_quadtree)

set(CMAKE_CXX_STANDARD 17)

add_executable(simple_quadtree main.cpp Box.h Vector2.h Quadtree.h)