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
CMAKE_SOURCE_DIR = /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/src/cmocka-1.0.1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests

# Include any dependencies generated for this target.
include example/CMakeFiles/simple_test.dir/depend.make

# Include the progress variables for this target.
include example/CMakeFiles/simple_test.dir/progress.make

# Include the compile flags for this target's objects.
include example/CMakeFiles/simple_test.dir/flags.make

example/CMakeFiles/simple_test.dir/simple_test.c.o: example/CMakeFiles/simple_test.dir/flags.make
example/CMakeFiles/simple_test.dir/simple_test.c.o: /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/src/cmocka-1.0.1/example/simple_test.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object example/CMakeFiles/simple_test.dir/simple_test.c.o"
	cd /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/example && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/simple_test.dir/simple_test.c.o   -c /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/src/cmocka-1.0.1/example/simple_test.c

example/CMakeFiles/simple_test.dir/simple_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/simple_test.dir/simple_test.c.i"
	cd /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/example && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/src/cmocka-1.0.1/example/simple_test.c > CMakeFiles/simple_test.dir/simple_test.c.i

example/CMakeFiles/simple_test.dir/simple_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/simple_test.dir/simple_test.c.s"
	cd /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/example && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/src/cmocka-1.0.1/example/simple_test.c -o CMakeFiles/simple_test.dir/simple_test.c.s

example/CMakeFiles/simple_test.dir/simple_test.c.o.requires:
.PHONY : example/CMakeFiles/simple_test.dir/simple_test.c.o.requires

example/CMakeFiles/simple_test.dir/simple_test.c.o.provides: example/CMakeFiles/simple_test.dir/simple_test.c.o.requires
	$(MAKE) -f example/CMakeFiles/simple_test.dir/build.make example/CMakeFiles/simple_test.dir/simple_test.c.o.provides.build
.PHONY : example/CMakeFiles/simple_test.dir/simple_test.c.o.provides

example/CMakeFiles/simple_test.dir/simple_test.c.o.provides.build: example/CMakeFiles/simple_test.dir/simple_test.c.o

# Object files for target simple_test
simple_test_OBJECTS = \
"CMakeFiles/simple_test.dir/simple_test.c.o"

# External object files for target simple_test
simple_test_EXTERNAL_OBJECTS =

example/simple_test: example/CMakeFiles/simple_test.dir/simple_test.c.o
example/simple_test: example/CMakeFiles/simple_test.dir/build.make
example/simple_test: src/libcmocka.so.0.3.1
example/simple_test: /usr/lib/x86_64-linux-gnu/librt.so
example/simple_test: example/CMakeFiles/simple_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable simple_test"
	cd /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/example && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simple_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/CMakeFiles/simple_test.dir/build: example/simple_test
.PHONY : example/CMakeFiles/simple_test.dir/build

example/CMakeFiles/simple_test.dir/requires: example/CMakeFiles/simple_test.dir/simple_test.c.o.requires
.PHONY : example/CMakeFiles/simple_test.dir/requires

example/CMakeFiles/simple_test.dir/clean:
	cd /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/example && $(CMAKE_COMMAND) -P CMakeFiles/simple_test.dir/cmake_clean.cmake
.PHONY : example/CMakeFiles/simple_test.dir/clean

example/CMakeFiles/simple_test.dir/depend:
	cd /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/src/cmocka-1.0.1 /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/src/cmocka-1.0.1/example /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/example /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/example/CMakeFiles/simple_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/CMakeFiles/simple_test.dir/depend
