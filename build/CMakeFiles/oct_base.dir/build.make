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
include CMakeFiles/oct_base.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/oct_base.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/oct_base.dir/flags.make

CMakeFiles/oct_base.dir/octopus/arp_server.cc.o: CMakeFiles/oct_base.dir/flags.make
CMakeFiles/oct_base.dir/octopus/arp_server.cc.o: ../octopus/arp_server.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/iplab/dash-sim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/oct_base.dir/octopus/arp_server.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/oct_base.dir/octopus/arp_server.cc.o -c /home/iplab/dash-sim/octopus/arp_server.cc

CMakeFiles/oct_base.dir/octopus/arp_server.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/oct_base.dir/octopus/arp_server.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/iplab/dash-sim/octopus/arp_server.cc > CMakeFiles/oct_base.dir/octopus/arp_server.cc.i

CMakeFiles/oct_base.dir/octopus/arp_server.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/oct_base.dir/octopus/arp_server.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/iplab/dash-sim/octopus/arp_server.cc -o CMakeFiles/oct_base.dir/octopus/arp_server.cc.s

CMakeFiles/oct_base.dir/octopus/arp_server.cc.o.requires:

.PHONY : CMakeFiles/oct_base.dir/octopus/arp_server.cc.o.requires

CMakeFiles/oct_base.dir/octopus/arp_server.cc.o.provides: CMakeFiles/oct_base.dir/octopus/arp_server.cc.o.requires
	$(MAKE) -f CMakeFiles/oct_base.dir/build.make CMakeFiles/oct_base.dir/octopus/arp_server.cc.o.provides.build
.PHONY : CMakeFiles/oct_base.dir/octopus/arp_server.cc.o.provides

CMakeFiles/oct_base.dir/octopus/arp_server.cc.o.provides.build: CMakeFiles/oct_base.dir/octopus/arp_server.cc.o


CMakeFiles/oct_base.dir/octopus/octopus_base.cc.o: CMakeFiles/oct_base.dir/flags.make
CMakeFiles/oct_base.dir/octopus/octopus_base.cc.o: ../octopus/octopus_base.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/iplab/dash-sim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/oct_base.dir/octopus/octopus_base.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/oct_base.dir/octopus/octopus_base.cc.o -c /home/iplab/dash-sim/octopus/octopus_base.cc

CMakeFiles/oct_base.dir/octopus/octopus_base.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/oct_base.dir/octopus/octopus_base.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/iplab/dash-sim/octopus/octopus_base.cc > CMakeFiles/oct_base.dir/octopus/octopus_base.cc.i

CMakeFiles/oct_base.dir/octopus/octopus_base.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/oct_base.dir/octopus/octopus_base.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/iplab/dash-sim/octopus/octopus_base.cc -o CMakeFiles/oct_base.dir/octopus/octopus_base.cc.s

CMakeFiles/oct_base.dir/octopus/octopus_base.cc.o.requires:

.PHONY : CMakeFiles/oct_base.dir/octopus/octopus_base.cc.o.requires

CMakeFiles/oct_base.dir/octopus/octopus_base.cc.o.provides: CMakeFiles/oct_base.dir/octopus/octopus_base.cc.o.requires
	$(MAKE) -f CMakeFiles/oct_base.dir/build.make CMakeFiles/oct_base.dir/octopus/octopus_base.cc.o.provides.build
.PHONY : CMakeFiles/oct_base.dir/octopus/octopus_base.cc.o.provides

CMakeFiles/oct_base.dir/octopus/octopus_base.cc.o.provides.build: CMakeFiles/oct_base.dir/octopus/octopus_base.cc.o


# Object files for target oct_base
oct_base_OBJECTS = \
"CMakeFiles/oct_base.dir/octopus/arp_server.cc.o" \
"CMakeFiles/oct_base.dir/octopus/octopus_base.cc.o"

# External object files for target oct_base
oct_base_EXTERNAL_OBJECTS =

liboct_base.a: CMakeFiles/oct_base.dir/octopus/arp_server.cc.o
liboct_base.a: CMakeFiles/oct_base.dir/octopus/octopus_base.cc.o
liboct_base.a: CMakeFiles/oct_base.dir/build.make
liboct_base.a: CMakeFiles/oct_base.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/iplab/dash-sim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library liboct_base.a"
	$(CMAKE_COMMAND) -P CMakeFiles/oct_base.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/oct_base.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/oct_base.dir/build: liboct_base.a

.PHONY : CMakeFiles/oct_base.dir/build

CMakeFiles/oct_base.dir/requires: CMakeFiles/oct_base.dir/octopus/arp_server.cc.o.requires
CMakeFiles/oct_base.dir/requires: CMakeFiles/oct_base.dir/octopus/octopus_base.cc.o.requires

.PHONY : CMakeFiles/oct_base.dir/requires

CMakeFiles/oct_base.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/oct_base.dir/cmake_clean.cmake
.PHONY : CMakeFiles/oct_base.dir/clean

CMakeFiles/oct_base.dir/depend:
	cd /home/iplab/dash-sim/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/iplab/dash-sim /home/iplab/dash-sim /home/iplab/dash-sim/build /home/iplab/dash-sim/build /home/iplab/dash-sim/build/CMakeFiles/oct_base.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/oct_base.dir/depend

