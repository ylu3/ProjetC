# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/luy/OneDrive/Documents/Ensimag/1A/S6/ProjetC/projet_c_ig

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/luy/OneDrive/Documents/Ensimag/1A/S6/ProjetC/projet_c_ig/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/parsing.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/parsing.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/parsing.dir/flags.make

CMakeFiles/parsing.dir/tests/parsing.c.o: CMakeFiles/parsing.dir/flags.make
CMakeFiles/parsing.dir/tests/parsing.c.o: ../tests/parsing.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/luy/OneDrive/Documents/Ensimag/1A/S6/ProjetC/projet_c_ig/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/parsing.dir/tests/parsing.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/parsing.dir/tests/parsing.c.o   -c /Users/luy/OneDrive/Documents/Ensimag/1A/S6/ProjetC/projet_c_ig/tests/parsing.c

CMakeFiles/parsing.dir/tests/parsing.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/parsing.dir/tests/parsing.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/luy/OneDrive/Documents/Ensimag/1A/S6/ProjetC/projet_c_ig/tests/parsing.c > CMakeFiles/parsing.dir/tests/parsing.c.i

CMakeFiles/parsing.dir/tests/parsing.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/parsing.dir/tests/parsing.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/luy/OneDrive/Documents/Ensimag/1A/S6/ProjetC/projet_c_ig/tests/parsing.c -o CMakeFiles/parsing.dir/tests/parsing.c.s

# Object files for target parsing
parsing_OBJECTS = \
"CMakeFiles/parsing.dir/tests/parsing.c.o"

# External object files for target parsing
parsing_EXTERNAL_OBJECTS =

parsing: CMakeFiles/parsing.dir/tests/parsing.c.o
parsing: CMakeFiles/parsing.dir/build.make
parsing: libei.a
parsing: ../_osx/libeibase.a
parsing: CMakeFiles/parsing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/luy/OneDrive/Documents/Ensimag/1A/S6/ProjetC/projet_c_ig/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable parsing"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/parsing.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/parsing.dir/build: parsing

.PHONY : CMakeFiles/parsing.dir/build

CMakeFiles/parsing.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/parsing.dir/cmake_clean.cmake
.PHONY : CMakeFiles/parsing.dir/clean

CMakeFiles/parsing.dir/depend:
	cd /Users/luy/OneDrive/Documents/Ensimag/1A/S6/ProjetC/projet_c_ig/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/luy/OneDrive/Documents/Ensimag/1A/S6/ProjetC/projet_c_ig /Users/luy/OneDrive/Documents/Ensimag/1A/S6/ProjetC/projet_c_ig /Users/luy/OneDrive/Documents/Ensimag/1A/S6/ProjetC/projet_c_ig/cmake-build-debug /Users/luy/OneDrive/Documents/Ensimag/1A/S6/ProjetC/projet_c_ig/cmake-build-debug /Users/luy/OneDrive/Documents/Ensimag/1A/S6/ProjetC/projet_c_ig/cmake-build-debug/CMakeFiles/parsing.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/parsing.dir/depend

