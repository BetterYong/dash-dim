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
include CMakeFiles/possion_sender.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/possion_sender.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/possion_sender.dir/flags.make

CMakeFiles/possion_sender.dir/tools/possion_sender.cc.o: CMakeFiles/possion_sender.dir/flags.make
CMakeFiles/possion_sender.dir/tools/possion_sender.cc.o: ../tools/possion_sender.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/iplab/dash-sim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/possion_sender.dir/tools/possion_sender.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/possion_sender.dir/tools/possion_sender.cc.o -c /home/iplab/dash-sim/tools/possion_sender.cc

CMakeFiles/possion_sender.dir/tools/possion_sender.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/possion_sender.dir/tools/possion_sender.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/iplab/dash-sim/tools/possion_sender.cc > CMakeFiles/possion_sender.dir/tools/possion_sender.cc.i

CMakeFiles/possion_sender.dir/tools/possion_sender.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/possion_sender.dir/tools/possion_sender.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/iplab/dash-sim/tools/possion_sender.cc -o CMakeFiles/possion_sender.dir/tools/possion_sender.cc.s

CMakeFiles/possion_sender.dir/tools/possion_sender.cc.o.requires:

.PHONY : CMakeFiles/possion_sender.dir/tools/possion_sender.cc.o.requires

CMakeFiles/possion_sender.dir/tools/possion_sender.cc.o.provides: CMakeFiles/possion_sender.dir/tools/possion_sender.cc.o.requires
	$(MAKE) -f CMakeFiles/possion_sender.dir/build.make CMakeFiles/possion_sender.dir/tools/possion_sender.cc.o.provides.build
.PHONY : CMakeFiles/possion_sender.dir/tools/possion_sender.cc.o.provides

CMakeFiles/possion_sender.dir/tools/possion_sender.cc.o.provides.build: CMakeFiles/possion_sender.dir/tools/possion_sender.cc.o


# Object files for target possion_sender
possion_sender_OBJECTS = \
"CMakeFiles/possion_sender.dir/tools/possion_sender.cc.o"

# External object files for target possion_sender
possion_sender_EXTERNAL_OBJECTS =

possion_sender: CMakeFiles/possion_sender.dir/tools/possion_sender.cc.o
possion_sender: CMakeFiles/possion_sender.dir/build.make
possion_sender: CMakeFiles/possion_sender.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/iplab/dash-sim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable possion_sender"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/possion_sender.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/possion_sender.dir/build: possion_sender

.PHONY : CMakeFiles/possion_sender.dir/build

CMakeFiles/possion_sender.dir/requires: CMakeFiles/possion_sender.dir/tools/possion_sender.cc.o.requires

.PHONY : CMakeFiles/possion_sender.dir/requires

CMakeFiles/possion_sender.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/possion_sender.dir/cmake_clean.cmake
.PHONY : CMakeFiles/possion_sender.dir/clean

CMakeFiles/possion_sender.dir/depend:
	cd /home/iplab/dash-sim/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/iplab/dash-sim /home/iplab/dash-sim /home/iplab/dash-sim/build /home/iplab/dash-sim/build /home/iplab/dash-sim/build/CMakeFiles/possion_sender.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/possion_sender.dir/depend

