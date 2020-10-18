cmake_minimum_required(VERSION 3.16)
project(xyzndatafile)

find_package(PCL 1.10.0 REQUIRED )

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable(xyzndatafile main.cpp)
target_compile_options(xyzndatafile PRIVATE -Wall)
target_link_libraries (xyzndatafile ${PCL_LIBRARIES})
