# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/alexdillon/Bureau/Google/Cours/L3/Tetris

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alexdillon/Bureau/Google/Cours/L3/Tetris/build

# Include any dependencies generated for this target.
include CMakeFiles/TetrisServer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TetrisServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TetrisServer.dir/flags.make

CMakeFiles/TetrisServer.dir/TetrisServer.cc.o: CMakeFiles/TetrisServer.dir/flags.make
CMakeFiles/TetrisServer.dir/TetrisServer.cc.o: ../TetrisServer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alexdillon/Bureau/Google/Cours/L3/Tetris/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TetrisServer.dir/TetrisServer.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TetrisServer.dir/TetrisServer.cc.o -c /home/alexdillon/Bureau/Google/Cours/L3/Tetris/TetrisServer.cc

CMakeFiles/TetrisServer.dir/TetrisServer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TetrisServer.dir/TetrisServer.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alexdillon/Bureau/Google/Cours/L3/Tetris/TetrisServer.cc > CMakeFiles/TetrisServer.dir/TetrisServer.cc.i

CMakeFiles/TetrisServer.dir/TetrisServer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TetrisServer.dir/TetrisServer.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alexdillon/Bureau/Google/Cours/L3/Tetris/TetrisServer.cc -o CMakeFiles/TetrisServer.dir/TetrisServer.cc.s

CMakeFiles/TetrisServer.dir/TetrisServer.cc.o.requires:

.PHONY : CMakeFiles/TetrisServer.dir/TetrisServer.cc.o.requires

CMakeFiles/TetrisServer.dir/TetrisServer.cc.o.provides: CMakeFiles/TetrisServer.dir/TetrisServer.cc.o.requires
	$(MAKE) -f CMakeFiles/TetrisServer.dir/build.make CMakeFiles/TetrisServer.dir/TetrisServer.cc.o.provides.build
.PHONY : CMakeFiles/TetrisServer.dir/TetrisServer.cc.o.provides

CMakeFiles/TetrisServer.dir/TetrisServer.cc.o.provides.build: CMakeFiles/TetrisServer.dir/TetrisServer.cc.o


# Object files for target TetrisServer
TetrisServer_OBJECTS = \
"CMakeFiles/TetrisServer.dir/TetrisServer.cc.o"

# External object files for target TetrisServer
TetrisServer_EXTERNAL_OBJECTS =

TetrisServer: CMakeFiles/TetrisServer.dir/TetrisServer.cc.o
TetrisServer: CMakeFiles/TetrisServer.dir/build.make
TetrisServer: /usr/lib/x86_64-linux-gnu/libboost_thread.so
TetrisServer: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
TetrisServer: /usr/lib/x86_64-linux-gnu/libboost_system.so
TetrisServer: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
TetrisServer: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
TetrisServer: /usr/lib/x86_64-linux-gnu/libpthread.so
TetrisServer: lib/libSerial.so
TetrisServer: CMakeFiles/TetrisServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alexdillon/Bureau/Google/Cours/L3/Tetris/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable TetrisServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TetrisServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TetrisServer.dir/build: TetrisServer

.PHONY : CMakeFiles/TetrisServer.dir/build

CMakeFiles/TetrisServer.dir/requires: CMakeFiles/TetrisServer.dir/TetrisServer.cc.o.requires

.PHONY : CMakeFiles/TetrisServer.dir/requires

CMakeFiles/TetrisServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TetrisServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TetrisServer.dir/clean

CMakeFiles/TetrisServer.dir/depend:
	cd /home/alexdillon/Bureau/Google/Cours/L3/Tetris/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alexdillon/Bureau/Google/Cours/L3/Tetris /home/alexdillon/Bureau/Google/Cours/L3/Tetris /home/alexdillon/Bureau/Google/Cours/L3/Tetris/build /home/alexdillon/Bureau/Google/Cours/L3/Tetris/build /home/alexdillon/Bureau/Google/Cours/L3/Tetris/build/CMakeFiles/TetrisServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TetrisServer.dir/depend

