# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/frederik/geant4/user/NC2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/frederik/geant4/user/NC2/build

# Utility rule file for NC_2.

# Include the progress variables for this target.
include CMakeFiles/NC_2.dir/progress.make

CMakeFiles/NC_2: NC2

NC_2: CMakeFiles/NC_2
NC_2: CMakeFiles/NC_2.dir/build.make
.PHONY : NC_2

# Rule to build all files generated by this target.
CMakeFiles/NC_2.dir/build: NC_2
.PHONY : CMakeFiles/NC_2.dir/build

CMakeFiles/NC_2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/NC_2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/NC_2.dir/clean

CMakeFiles/NC_2.dir/depend:
	cd /home/frederik/geant4/user/NC2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/frederik/geant4/user/NC2 /home/frederik/geant4/user/NC2 /home/frederik/geant4/user/NC2/build /home/frederik/geant4/user/NC2/build /home/frederik/geant4/user/NC2/build/CMakeFiles/NC_2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/NC_2.dir/depend
