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
include example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/depend.make

# Include the progress variables for this target.
include example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/progress.make

# Include the compile flags for this target's objects.
include example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/flags.make

example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.o: example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/flags.make
example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.o: /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/src/cmocka-1.0.1/example/chef_wrap/waiter_test_wrap.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.o"
	cd /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/example/chef_wrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.o   -c /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/src/cmocka-1.0.1/example/chef_wrap/waiter_test_wrap.c

example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.i"
	cd /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/example/chef_wrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/src/cmocka-1.0.1/example/chef_wrap/waiter_test_wrap.c > CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.i

example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.s"
	cd /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/example/chef_wrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/src/cmocka-1.0.1/example/chef_wrap/waiter_test_wrap.c -o CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.s

example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.o.requires:
.PHONY : example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.o.requires

example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.o.provides: example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.o.requires
	$(MAKE) -f example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/build.make example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.o.provides.build
.PHONY : example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.o.provides

example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.o.provides.build: example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.o

example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/chef.c.o: example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/flags.make
example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/chef.c.o: /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/src/cmocka-1.0.1/example/chef_wrap/chef.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/chef.c.o"
	cd /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/example/chef_wrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/waiter_test_wrap.dir/chef.c.o   -c /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/src/cmocka-1.0.1/example/chef_wrap/chef.c

example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/chef.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/waiter_test_wrap.dir/chef.c.i"
	cd /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/example/chef_wrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/src/cmocka-1.0.1/example/chef_wrap/chef.c > CMakeFiles/waiter_test_wrap.dir/chef.c.i

example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/chef.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/waiter_test_wrap.dir/chef.c.s"
	cd /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/example/chef_wrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/src/cmocka-1.0.1/example/chef_wrap/chef.c -o CMakeFiles/waiter_test_wrap.dir/chef.c.s

example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/chef.c.o.requires:
.PHONY : example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/chef.c.o.requires

example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/chef.c.o.provides: example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/chef.c.o.requires
	$(MAKE) -f example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/build.make example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/chef.c.o.provides.build
.PHONY : example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/chef.c.o.provides

example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/chef.c.o.provides.build: example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/chef.c.o

# Object files for target waiter_test_wrap
waiter_test_wrap_OBJECTS = \
"CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.o" \
"CMakeFiles/waiter_test_wrap.dir/chef.c.o"

# External object files for target waiter_test_wrap
waiter_test_wrap_EXTERNAL_OBJECTS =

example/chef_wrap/waiter_test_wrap: example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.o
example/chef_wrap/waiter_test_wrap: example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/chef.c.o
example/chef_wrap/waiter_test_wrap: example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/build.make
example/chef_wrap/waiter_test_wrap: src/libcmocka.so.0.3.1
example/chef_wrap/waiter_test_wrap: /usr/lib/x86_64-linux-gnu/librt.so
example/chef_wrap/waiter_test_wrap: example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable waiter_test_wrap"
	cd /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/example/chef_wrap && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/waiter_test_wrap.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/build: example/chef_wrap/waiter_test_wrap
.PHONY : example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/build

example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/requires: example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/waiter_test_wrap.c.o.requires
example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/requires: example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/chef.c.o.requires
.PHONY : example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/requires

example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/clean:
	cd /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/example/chef_wrap && $(CMAKE_COMMAND) -P CMakeFiles/waiter_test_wrap.dir/cmake_clean.cmake
.PHONY : example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/clean

example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/depend:
	cd /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/src/cmocka-1.0.1 /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/src/cmocka-1.0.1/example/chef_wrap /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/example/chef_wrap /home/majki/Dokumenty/FinalVersion/spellcheck-mj348711/Tests/example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/chef_wrap/CMakeFiles/waiter_test_wrap.dir/depend

