# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /home/ben/Downloads/clion-2017.2/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/ben/Downloads/clion-2017.2/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ben/CLionProjects/Websocket-server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ben/CLionProjects/Websocket-server/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Websocket_server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Websocket_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Websocket_server.dir/flags.make

CMakeFiles/Websocket_server.dir/main.cpp.o: CMakeFiles/Websocket_server.dir/flags.make
CMakeFiles/Websocket_server.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ben/CLionProjects/Websocket-server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Websocket_server.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Websocket_server.dir/main.cpp.o -c /home/ben/CLionProjects/Websocket-server/main.cpp

CMakeFiles/Websocket_server.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Websocket_server.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ben/CLionProjects/Websocket-server/main.cpp > CMakeFiles/Websocket_server.dir/main.cpp.i

CMakeFiles/Websocket_server.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Websocket_server.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ben/CLionProjects/Websocket-server/main.cpp -o CMakeFiles/Websocket_server.dir/main.cpp.s

CMakeFiles/Websocket_server.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/Websocket_server.dir/main.cpp.o.requires

CMakeFiles/Websocket_server.dir/main.cpp.o.provides: CMakeFiles/Websocket_server.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Websocket_server.dir/build.make CMakeFiles/Websocket_server.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/Websocket_server.dir/main.cpp.o.provides

CMakeFiles/Websocket_server.dir/main.cpp.o.provides.build: CMakeFiles/Websocket_server.dir/main.cpp.o


# Object files for target Websocket_server
Websocket_server_OBJECTS = \
"CMakeFiles/Websocket_server.dir/main.cpp.o"

# External object files for target Websocket_server
Websocket_server_EXTERNAL_OBJECTS =

Websocket_server: CMakeFiles/Websocket_server.dir/main.cpp.o
Websocket_server: CMakeFiles/Websocket_server.dir/build.make
Websocket_server: /usr/lib/libuWS.so
Websocket_server: CMakeFiles/Websocket_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ben/CLionProjects/Websocket-server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Websocket_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Websocket_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Websocket_server.dir/build: Websocket_server

.PHONY : CMakeFiles/Websocket_server.dir/build

CMakeFiles/Websocket_server.dir/requires: CMakeFiles/Websocket_server.dir/main.cpp.o.requires

.PHONY : CMakeFiles/Websocket_server.dir/requires

CMakeFiles/Websocket_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Websocket_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Websocket_server.dir/clean

CMakeFiles/Websocket_server.dir/depend:
	cd /home/ben/CLionProjects/Websocket-server/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ben/CLionProjects/Websocket-server /home/ben/CLionProjects/Websocket-server /home/ben/CLionProjects/Websocket-server/cmake-build-debug /home/ben/CLionProjects/Websocket-server/cmake-build-debug /home/ben/CLionProjects/Websocket-server/cmake-build-debug/CMakeFiles/Websocket_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Websocket_server.dir/depend

