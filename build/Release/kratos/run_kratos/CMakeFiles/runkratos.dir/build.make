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
CMAKE_SOURCE_DIR = /home/jig/Kratos

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jig/Kratos/build/Release

# Include any dependencies generated for this target.
include kratos/run_kratos/CMakeFiles/runkratos.dir/depend.make

# Include the progress variables for this target.
include kratos/run_kratos/CMakeFiles/runkratos.dir/progress.make

# Include the compile flags for this target's objects.
include kratos/run_kratos/CMakeFiles/runkratos.dir/flags.make

kratos/run_kratos/CMakeFiles/runkratos.dir/krun_main.cpp.o: kratos/run_kratos/CMakeFiles/runkratos.dir/flags.make
kratos/run_kratos/CMakeFiles/runkratos.dir/krun_main.cpp.o: ../../kratos/run_kratos/krun_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jig/Kratos/build/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object kratos/run_kratos/CMakeFiles/runkratos.dir/krun_main.cpp.o"
	cd /home/jig/Kratos/build/Release/kratos/run_kratos && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/runkratos.dir/krun_main.cpp.o -c /home/jig/Kratos/kratos/run_kratos/krun_main.cpp

kratos/run_kratos/CMakeFiles/runkratos.dir/krun_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/runkratos.dir/krun_main.cpp.i"
	cd /home/jig/Kratos/build/Release/kratos/run_kratos && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jig/Kratos/kratos/run_kratos/krun_main.cpp > CMakeFiles/runkratos.dir/krun_main.cpp.i

kratos/run_kratos/CMakeFiles/runkratos.dir/krun_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/runkratos.dir/krun_main.cpp.s"
	cd /home/jig/Kratos/build/Release/kratos/run_kratos && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jig/Kratos/kratos/run_kratos/krun_main.cpp -o CMakeFiles/runkratos.dir/krun_main.cpp.s

kratos/run_kratos/CMakeFiles/runkratos.dir/krun_main.cpp.o.requires:

.PHONY : kratos/run_kratos/CMakeFiles/runkratos.dir/krun_main.cpp.o.requires

kratos/run_kratos/CMakeFiles/runkratos.dir/krun_main.cpp.o.provides: kratos/run_kratos/CMakeFiles/runkratos.dir/krun_main.cpp.o.requires
	$(MAKE) -f kratos/run_kratos/CMakeFiles/runkratos.dir/build.make kratos/run_kratos/CMakeFiles/runkratos.dir/krun_main.cpp.o.provides.build
.PHONY : kratos/run_kratos/CMakeFiles/runkratos.dir/krun_main.cpp.o.provides

kratos/run_kratos/CMakeFiles/runkratos.dir/krun_main.cpp.o.provides.build: kratos/run_kratos/CMakeFiles/runkratos.dir/krun_main.cpp.o


# Object files for target runkratos
runkratos_OBJECTS = \
"CMakeFiles/runkratos.dir/krun_main.cpp.o"

# External object files for target runkratos
runkratos_EXTERNAL_OBJECTS =

kratos/run_kratos/runkratos: kratos/run_kratos/CMakeFiles/runkratos.dir/krun_main.cpp.o
kratos/run_kratos/runkratos: kratos/run_kratos/CMakeFiles/runkratos.dir/build.make
kratos/run_kratos/runkratos: /usr/lib/x86_64-linux-gnu/libpython3.6m.so.1.0
kratos/run_kratos/runkratos: kratos/run_kratos/CMakeFiles/runkratos.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jig/Kratos/build/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable runkratos"
	cd /home/jig/Kratos/build/Release/kratos/run_kratos && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/runkratos.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
kratos/run_kratos/CMakeFiles/runkratos.dir/build: kratos/run_kratos/runkratos

.PHONY : kratos/run_kratos/CMakeFiles/runkratos.dir/build

kratos/run_kratos/CMakeFiles/runkratos.dir/requires: kratos/run_kratos/CMakeFiles/runkratos.dir/krun_main.cpp.o.requires

.PHONY : kratos/run_kratos/CMakeFiles/runkratos.dir/requires

kratos/run_kratos/CMakeFiles/runkratos.dir/clean:
	cd /home/jig/Kratos/build/Release/kratos/run_kratos && $(CMAKE_COMMAND) -P CMakeFiles/runkratos.dir/cmake_clean.cmake
.PHONY : kratos/run_kratos/CMakeFiles/runkratos.dir/clean

kratos/run_kratos/CMakeFiles/runkratos.dir/depend:
	cd /home/jig/Kratos/build/Release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jig/Kratos /home/jig/Kratos/kratos/run_kratos /home/jig/Kratos/build/Release /home/jig/Kratos/build/Release/kratos/run_kratos /home/jig/Kratos/build/Release/kratos/run_kratos/CMakeFiles/runkratos.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : kratos/run_kratos/CMakeFiles/runkratos.dir/depend

