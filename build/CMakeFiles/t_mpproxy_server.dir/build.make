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
include CMakeFiles/t_mpproxy_server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/t_mpproxy_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/t_mpproxy_server.dir/flags.make

CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.o: CMakeFiles/t_mpproxy_server.dir/flags.make
CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.o: ../tpproxy/mpproxy_server_test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zsy/dash-sim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.o -c /home/zsy/dash-sim/tpproxy/mpproxy_server_test.cc

CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zsy/dash-sim/tpproxy/mpproxy_server_test.cc > CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.i

CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zsy/dash-sim/tpproxy/mpproxy_server_test.cc -o CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.s

CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.o.requires:

.PHONY : CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.o.requires

CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.o.provides: CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.o.requires
	$(MAKE) -f CMakeFiles/t_mpproxy_server.dir/build.make CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.o.provides.build
.PHONY : CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.o.provides

CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.o.provides.build: CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.o


# Object files for target t_mpproxy_server
t_mpproxy_server_OBJECTS = \
"CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.o"

# External object files for target t_mpproxy_server
t_mpproxy_server_EXTERNAL_OBJECTS =

t_mpproxy_server: CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.o
t_mpproxy_server: CMakeFiles/t_mpproxy_server.dir/build.make
t_mpproxy_server: libproxy.a
t_mpproxy_server: sequencer/libsequencer.a
t_mpproxy_server: base/libbase.a
t_mpproxy_server: CMakeFiles/t_mpproxy_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zsy/dash-sim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable t_mpproxy_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/t_mpproxy_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/t_mpproxy_server.dir/build: t_mpproxy_server

.PHONY : CMakeFiles/t_mpproxy_server.dir/build

CMakeFiles/t_mpproxy_server.dir/requires: CMakeFiles/t_mpproxy_server.dir/tpproxy/mpproxy_server_test.cc.o.requires

.PHONY : CMakeFiles/t_mpproxy_server.dir/requires

CMakeFiles/t_mpproxy_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/t_mpproxy_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/t_mpproxy_server.dir/clean

CMakeFiles/t_mpproxy_server.dir/depend:
	cd /home/zsy/dash-sim/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zsy/dash-sim /home/zsy/dash-sim /home/zsy/dash-sim/build /home/zsy/dash-sim/build /home/zsy/dash-sim/build/CMakeFiles/t_mpproxy_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/t_mpproxy_server.dir/depend

