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
CMAKE_SOURCE_DIR = /home/zsy/dash-sim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zsy/dash-sim/build

# Include any dependencies generated for this target.
include CMakeFiles/c_hen.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/c_hen.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/c_hen.dir/flags.make

CMakeFiles/c_hen.dir/hen/c_hen.cc.o: CMakeFiles/c_hen.dir/flags.make
CMakeFiles/c_hen.dir/hen/c_hen.cc.o: ../hen/c_hen.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zsy/dash-sim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/c_hen.dir/hen/c_hen.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/c_hen.dir/hen/c_hen.cc.o -c /home/zsy/dash-sim/hen/c_hen.cc

CMakeFiles/c_hen.dir/hen/c_hen.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/c_hen.dir/hen/c_hen.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zsy/dash-sim/hen/c_hen.cc > CMakeFiles/c_hen.dir/hen/c_hen.cc.i

CMakeFiles/c_hen.dir/hen/c_hen.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/c_hen.dir/hen/c_hen.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zsy/dash-sim/hen/c_hen.cc -o CMakeFiles/c_hen.dir/hen/c_hen.cc.s

CMakeFiles/c_hen.dir/hen/c_hen.cc.o.requires:

.PHONY : CMakeFiles/c_hen.dir/hen/c_hen.cc.o.requires

CMakeFiles/c_hen.dir/hen/c_hen.cc.o.provides: CMakeFiles/c_hen.dir/hen/c_hen.cc.o.requires
	$(MAKE) -f CMakeFiles/c_hen.dir/build.make CMakeFiles/c_hen.dir/hen/c_hen.cc.o.provides.build
.PHONY : CMakeFiles/c_hen.dir/hen/c_hen.cc.o.provides

CMakeFiles/c_hen.dir/hen/c_hen.cc.o.provides.build: CMakeFiles/c_hen.dir/hen/c_hen.cc.o


# Object files for target c_hen
c_hen_OBJECTS = \
"CMakeFiles/c_hen.dir/hen/c_hen.cc.o"

# External object files for target c_hen
c_hen_EXTERNAL_OBJECTS =

c_hen: CMakeFiles/c_hen.dir/hen/c_hen.cc.o
c_hen: CMakeFiles/c_hen.dir/build.make
c_hen: base/libbase.a
c_hen: CMakeFiles/c_hen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zsy/dash-sim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable c_hen"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/c_hen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/c_hen.dir/build: c_hen

.PHONY : CMakeFiles/c_hen.dir/build

CMakeFiles/c_hen.dir/requires: CMakeFiles/c_hen.dir/hen/c_hen.cc.o.requires

.PHONY : CMakeFiles/c_hen.dir/requires

CMakeFiles/c_hen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/c_hen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/c_hen.dir/clean

CMakeFiles/c_hen.dir/depend:
	cd /home/zsy/dash-sim/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zsy/dash-sim /home/zsy/dash-sim /home/zsy/dash-sim/build /home/zsy/dash-sim/build /home/zsy/dash-sim/build/CMakeFiles/c_hen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/c_hen.dir/depend

