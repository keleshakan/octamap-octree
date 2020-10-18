cmake_minimum_required(VERSION 3.16)
project(pcl_compress)

find_package(PCL 1.10.0 REQUIRED )

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

set(CMAKE_CXX_STANDARD 17)

add_executable(PCL_Cloud_Viewer main.cpp)
target_compile_options(PCL_Cloud_Viewer PRIVATE -Wall)
target_link_libraries (PCL_Cloud_Viewer ${PCL_LIBRARIES})
