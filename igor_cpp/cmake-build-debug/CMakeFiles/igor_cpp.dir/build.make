# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/129/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/129/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hakan/CLionProjects/igor_cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hakan/CLionProjects/igor_cpp/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/igor_cpp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/igor_cpp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/igor_cpp.dir/flags.make

CMakeFiles/igor_cpp.dir/main.cpp.o: CMakeFiles/igor_cpp.dir/flags.make
CMakeFiles/igor_cpp.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hakan/CLionProjects/igor_cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/igor_cpp.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/igor_cpp.dir/main.cpp.o -c /home/hakan/CLionProjects/igor_cpp/main.cpp

CMakeFiles/igor_cpp.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/igor_cpp.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hakan/CLionProjects/igor_cpp/main.cpp > CMakeFiles/igor_cpp.dir/main.cpp.i

CMakeFiles/igor_cpp.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/igor_cpp.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hakan/CLionProjects/igor_cpp/main.cpp -o CMakeFiles/igor_cpp.dir/main.cpp.s

# Object files for target igor_cpp
igor_cpp_OBJECTS = \
"CMakeFiles/igor_cpp.dir/main.cpp.o"

# External object files for target igor_cpp
igor_cpp_EXTERNAL_OBJECTS =

igor_cpp: CMakeFiles/igor_cpp.dir/main.cpp.o
igor_cpp: CMakeFiles/igor_cpp.dir/build.make
igor_cpp: CMakeFiles/igor_cpp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hakan/CLionProjects/igor_cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable igor_cpp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/igor_cpp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/igor_cpp.dir/build: igor_cpp

.PHONY : CMakeFiles/igor_cpp.dir/build

CMakeFiles/igor_cpp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/igor_cpp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/igor_cpp.dir/clean

CMakeFiles/igor_cpp.dir/depend:
	cd /home/hakan/CLionProjects/igor_cpp/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hakan/CLionProjects/igor_cpp /home/hakan/CLionProjects/igor_cpp /home/hakan/CLionProjects/igor_cpp/cmake-build-debug /home/hakan/CLionProjects/igor_cpp/cmake-build-debug /home/hakan/CLionProjects/igor_cpp/cmake-build-debug/CMakeFiles/igor_cpp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/igor_cpp.dir/depend

