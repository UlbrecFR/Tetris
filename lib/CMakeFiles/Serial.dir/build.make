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
CMAKE_SOURCE_DIR = /home/alexdillon/Bureau/Google/Cours/L3/Tetris/lib

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alexdillon/Bureau/Google/Cours/L3/Tetris/lib

# Include any dependencies generated for this target.
include CMakeFiles/Serial.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Serial.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Serial.dir/flags.make

CMakeFiles/Serial.dir/Serial.cc.o: CMakeFiles/Serial.dir/flags.make
CMakeFiles/Serial.dir/Serial.cc.o: Serial.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alexdillon/Bureau/Google/Cours/L3/Tetris/lib/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Serial.dir/Serial.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serial.dir/Serial.cc.o -c /home/alexdillon/Bureau/Google/Cours/L3/Tetris/lib/Serial.cc

CMakeFiles/Serial.dir/Serial.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serial.dir/Serial.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alexdillon/Bureau/Google/Cours/L3/Tetris/lib/Serial.cc > CMakeFiles/Serial.dir/Serial.cc.i

CMakeFiles/Serial.dir/Serial.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serial.dir/Serial.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alexdillon/Bureau/Google/Cours/L3/Tetris/lib/Serial.cc -o CMakeFiles/Serial.dir/Serial.cc.s

CMakeFiles/Serial.dir/Serial.cc.o.requires:

.PHONY : CMakeFiles/Serial.dir/Serial.cc.o.requires

CMakeFiles/Serial.dir/Serial.cc.o.provides: CMakeFiles/Serial.dir/Serial.cc.o.requires
	$(MAKE) -f CMakeFiles/Serial.dir/build.make CMakeFiles/Serial.dir/Serial.cc.o.provides.build
.PHONY : CMakeFiles/Serial.dir/Serial.cc.o.provides

CMakeFiles/Serial.dir/Serial.cc.o.provides.build: CMakeFiles/Serial.dir/Serial.cc.o


# Object files for target Serial
Serial_OBJECTS = \
"CMakeFiles/Serial.dir/Serial.cc.o"

# External object files for target Serial
Serial_EXTERNAL_OBJECTS =

libSerial.so: CMakeFiles/Serial.dir/Serial.cc.o
libSerial.so: CMakeFiles/Serial.dir/build.make
libSerial.so: CMakeFiles/Serial.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alexdillon/Bureau/Google/Cours/L3/Tetris/lib/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libSerial.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Serial.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Serial.dir/build: libSerial.so

.PHONY : CMakeFiles/Serial.dir/build

CMakeFiles/Serial.dir/requires: CMakeFiles/Serial.dir/Serial.cc.o.requires

.PHONY : CMakeFiles/Serial.dir/requires

CMakeFiles/Serial.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Serial.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Serial.dir/clean

CMakeFiles/Serial.dir/depend:
	cd /home/alexdillon/Bureau/Google/Cours/L3/Tetris/lib && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alexdillon/Bureau/Google/Cours/L3/Tetris/lib /home/alexdillon/Bureau/Google/Cours/L3/Tetris/lib /home/alexdillon/Bureau/Google/Cours/L3/Tetris/lib /home/alexdillon/Bureau/Google/Cours/L3/Tetris/lib /home/alexdillon/Bureau/Google/Cours/L3/Tetris/lib/CMakeFiles/Serial.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Serial.dir/depend

