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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /usr/local/dev/samples/hello_cpp_world

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /usr/local/dev/samples/hello_cpp_world

# Utility rule file for hello_cpp_world.vpk.

# Include the progress variables for this target.
include CMakeFiles/hello_cpp_world.vpk.dir/progress.make

CMakeFiles/hello_cpp_world.vpk: hello_cpp_world.vpk.out
	/usr/bin/cmake -E copy /usr/local/dev/samples/hello_cpp_world/hello_cpp_world.vpk.out hello_cpp_world.vpk

hello_cpp_world.vpk.out: hello_cpp_world.vpk_param.sfo
hello_cpp_world.vpk.out: hello_cpp_world.self
hello_cpp_world.vpk.out: sce_sys/icon0.png
hello_cpp_world.vpk.out: sce_sys/livearea/contents/bg.png
hello_cpp_world.vpk.out: sce_sys/livearea/contents/startup.png
hello_cpp_world.vpk.out: sce_sys/livearea/contents/template.xml
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/usr/local/dev/samples/hello_cpp_world/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building vpk hello_cpp_world.vpk"
	/usr/local/vitasdk/bin/vita-pack-vpk -a /usr/local/dev/samples/hello_cpp_world/sce_sys/icon0.png=sce_sys/icon0.png -a /usr/local/dev/samples/hello_cpp_world/sce_sys/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png -a /usr/local/dev/samples/hello_cpp_world/sce_sys/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png -a /usr/local/dev/samples/hello_cpp_world/sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml -s /usr/local/dev/samples/hello_cpp_world/hello_cpp_world.vpk_param.sfo -b /usr/local/dev/samples/hello_cpp_world/hello_cpp_world.self /usr/local/dev/samples/hello_cpp_world/hello_cpp_world.vpk.out

hello_cpp_world.vpk_param.sfo: hello_cpp_world.self
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/usr/local/dev/samples/hello_cpp_world/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating param.sfo for hello_cpp_world.vpk"
	/usr/local/vitasdk/bin/vita-mksfoex -d PARENTAL_LEVEL=1 -s APP_VER=01.00 -s TITLE_ID=VSDK00006 Hello\ World /usr/local/dev/samples/hello_cpp_world/hello_cpp_world.vpk_param.sfo

hello_cpp_world.vpk: CMakeFiles/hello_cpp_world.vpk
hello_cpp_world.vpk: hello_cpp_world.vpk.out
hello_cpp_world.vpk: hello_cpp_world.vpk_param.sfo
hello_cpp_world.vpk: CMakeFiles/hello_cpp_world.vpk.dir/build.make

.PHONY : hello_cpp_world.vpk

# Rule to build all files generated by this target.
CMakeFiles/hello_cpp_world.vpk.dir/build: hello_cpp_world.vpk

.PHONY : CMakeFiles/hello_cpp_world.vpk.dir/build

CMakeFiles/hello_cpp_world.vpk.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hello_cpp_world.vpk.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hello_cpp_world.vpk.dir/clean

CMakeFiles/hello_cpp_world.vpk.dir/depend:
	cd /usr/local/dev/samples/hello_cpp_world && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /usr/local/dev/samples/hello_cpp_world /usr/local/dev/samples/hello_cpp_world /usr/local/dev/samples/hello_cpp_world /usr/local/dev/samples/hello_cpp_world /usr/local/dev/samples/hello_cpp_world/CMakeFiles/hello_cpp_world.vpk.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hello_cpp_world.vpk.dir/depend

