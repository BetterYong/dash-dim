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
CMAKE_SOURCE_DIR = /home/iplab/dash-sim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/iplab/dash-sim/build

# Include any dependencies generated for this target.
include CMakeFiles/f_server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/f_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/f_server.dir/flags.make

CMakeFiles/f_server.dir/tools/f_server.cc.o: CMakeFiles/f_server.dir/flags.make
CMakeFiles/f_server.dir/tools/f_server.cc.o: ../tools/f_server.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/iplab/dash-sim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/f_server.dir/tools/f_server.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/f_server.dir/tools/f_server.cc.o -c /home/iplab/dash-sim/tools/f_server.cc

CMakeFiles/f_server.dir/tools/f_server.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/f_server.dir/tools/f_server.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/iplab/dash-sim/tools/f_server.cc > CMakeFiles/f_server.dir/tools/f_server.cc.i

CMakeFiles/f_server.dir/tools/f_server.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/f_server.dir/tools/f_server.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/iplab/dash-sim/tools/f_server.cc -o CMakeFiles/f_server.dir/tools/f_server.cc.s

CMakeFiles/f_server.dir/tools/f_server.cc.o.requires:

.PHONY : CMakeFiles/f_server.dir/tools/f_server.cc.o.requires

CMakeFiles/f_server.dir/tools/f_server.cc.o.provides: CMakeFiles/f_server.dir/tools/f_server.cc.o.requires
	$(MAKE) -f CMakeFiles/f_server.dir/build.make CMakeFiles/f_server.dir/tools/f_server.cc.o.provides.build
.PHONY : CMakeFiles/f_server.dir/tools/f_server.cc.o.provides

CMakeFiles/f_server.dir/tools/f_server.cc.o.provides.build: CMakeFiles/f_server.dir/tools/f_server.cc.o


# Object files for target f_server
f_server_OBJECTS = \
"CMakeFiles/f_server.dir/tools/f_server.cc.o"

# External object files for target f_server
f_server_EXTERNAL_OBJECTS =

f_server: CMakeFiles/f_server.dir/tools/f_server.cc.o
f_server: CMakeFiles/f_server.dir/build.make
f_server: base/libbase.a
f_server: CMakeFiles/f_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/iplab/dash-sim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable f_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/f_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/f_server.dir/build: f_server

.PHONY : CMakeFiles/f_server.dir/build

CMakeFiles/f_server.dir/requires: CMakeFiles/f_server.dir/tools/f_server.cc.o.requires

.PHONY : CMakeFiles/f_server.dir/requires

CMakeFiles/f_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/f_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/f_server.dir/clean

CMakeFiles/f_server.dir/depend:
	cd /home/iplab/dash-sim/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/iplab/dash-sim /home/iplab/dash-sim /home/iplab/dash-sim/build /home/iplab/dash-sim/build /home/iplab/dash-sim/build/CMakeFiles/f_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/f_server.dir/depend
