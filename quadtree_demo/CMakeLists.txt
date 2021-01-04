cmake_minimum_required(VERSION 3.16)
project (quadtree_rep)

find_package(OpenGL REQUIRED)

# Ask the user to give the paths to GLFW, GLEW and GLM.
# On Linux this can be found automatically with pkg-config;
# see http://stackoverflow.com/questions/6889264/cmake-flags-for-opengl-using-glew-and-glfw, for instance.

set(GLFW_INCLUDE_PATH "" CACHE PATH "The directory that contains GL/glfw.h" )
set(GLFW_LIBRARY "" CACHE FILEPATH "libglfw.a or glfw.lib" )

set(GLUT_INCLUDE_PATH "" CACHE PATH "The directory that contains GL/glut.h" )
set(GLUT_LIBRARY "" CACHE FILEPATH "libglut.a or glut.lib" )

set(GLEW_INCLUDE_PATH "" CACHE PATH "The directory that contains GL/glew.h" )
set(GLEW_LIBRARY "" CACHE FILEPATH "libglew.a or glew.lib" )

set(GLM_INCLUDE_PATH "" CACHE PATH "The directory that contains glm/glm.hpp" )

set(MY_HEADER_FILES
        # put all your .h here. Right now we have none.
        # Example :
        # ../common/shader.hpp
        quadtree.h
        )

set(MY_SOURCE_FILES
        # put all your .c/.cpp here.
        main.cpp.cpp
        # ../common/shader.cpp
        )



include_directories(
        ${GLFW_INCLUDE_PATH}
        ${GLEW_INCLUDE_PATH}
        ${GLM_INCLUDE_PATH}
        ${GLUT_INCLUDE_PATH}
)

add_executable(quadtree_rep main.cpp)
target_link_libraries(quadtree_rep
        ${OPENGL_LIBRARY} # filled by "find_package(OpenGL REQUIRED)"
        ${GLFW_LIBRARY}
        ${GLEW_LIBRARY}
        ${GLUT_LIBRARY}
        )

# HOW-TO. You can safely remove anything below.

# test if linux, mac or windows :

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    message("You're on Mac OS !")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    message("You're on Linux !")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message("You're on Windows !")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # Yes this is weird but you have to repeat the "if" here.