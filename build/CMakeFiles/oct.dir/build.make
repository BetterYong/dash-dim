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
include CMakeFiles/oct.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/oct.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/oct.dir/flags.make

CMakeFiles/oct.dir/octopus/octopus.cc.o: CMakeFiles/oct.dir/flags.make
CMakeFiles/oct.dir/octopus/octopus.cc.o: ../octopus/octopus.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/iplab/dash-sim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/oct.dir/octopus/octopus.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/oct.dir/octopus/octopus.cc.o -c /home/iplab/dash-sim/octopus/octopus.cc

CMakeFiles/oct.dir/octopus/octopus.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/oct.dir/octopus/octopus.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/iplab/dash-sim/octopus/octopus.cc > CMakeFiles/oct.dir/octopus/octopus.cc.i

CMakeFiles/oct.dir/octopus/octopus.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/oct.dir/octopus/octopus.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/iplab/dash-sim/octopus/octopus.cc -o CMakeFiles/oct.dir/octopus/octopus.cc.s

CMakeFiles/oct.dir/octopus/octopus.cc.o.requires:

.PHONY : CMakeFiles/oct.dir/octopus/octopus.cc.o.requires

CMakeFiles/oct.dir/octopus/octopus.cc.o.provides: CMakeFiles/oct.dir/octopus/octopus.cc.o.requires
	$(MAKE) -f CMakeFiles/oct.dir/build.make CMakeFiles/oct.dir/octopus/octopus.cc.o.provides.build
.PHONY : CMakeFiles/oct.dir/octopus/octopus.cc.o.provides

CMakeFiles/oct.dir/octopus/octopus.cc.o.provides.build: CMakeFiles/oct.dir/octopus/octopus.cc.o


# Object files for target oct
oct_OBJECTS = \
"CMakeFiles/oct.dir/octopus/octopus.cc.o"

# External object files for target oct
oct_EXTERNAL_OBJECTS =

oct: CMakeFiles/oct.dir/octopus/octopus.cc.o
oct: CMakeFiles/oct.dir/build.make
oct: liboct_base.a
oct: base/libbase.a
oct: CMakeFiles/oct.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/iplab/dash-sim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable oct"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/oct.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/oct.dir/build: oct

.PHONY : CMakeFiles/oct.dir/build

CMakeFiles/oct.dir/requires: CMakeFiles/oct.dir/octopus/octopus.cc.o.requires

.PHONY : CMakeFiles/oct.dir/requires

CMakeFiles/oct.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/oct.dir/cmake_clean.cmake
.PHONY : CMakeFiles/oct.dir/clean

CMakeFiles/oct.dir/depend:
	cd /home/iplab/dash-sim/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/iplab/dash-sim /home/iplab/dash-sim /home/iplab/dash-sim/build /home/iplab/dash-sim/build /home/iplab/dash-sim/build/CMakeFiles/oct.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/oct.dir/depend

