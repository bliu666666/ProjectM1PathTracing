# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/amine/Documents/Cours/M1/PPN_RayTracing

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/amine/Documents/Cours/M1/PPN_RayTracing/build

# Include any dependencies generated for this target.
include CMakeFiles/RayTracer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/RayTracer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/RayTracer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RayTracer.dir/flags.make

CMakeFiles/RayTracer.dir/src/main.cpp.o: CMakeFiles/RayTracer.dir/flags.make
CMakeFiles/RayTracer.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/RayTracer.dir/src/main.cpp.o: CMakeFiles/RayTracer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine/Documents/Cours/M1/PPN_RayTracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/RayTracer.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracer.dir/src/main.cpp.o -MF CMakeFiles/RayTracer.dir/src/main.cpp.o.d -o CMakeFiles/RayTracer.dir/src/main.cpp.o -c /home/amine/Documents/Cours/M1/PPN_RayTracing/src/main.cpp

CMakeFiles/RayTracer.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracer.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine/Documents/Cours/M1/PPN_RayTracing/src/main.cpp > CMakeFiles/RayTracer.dir/src/main.cpp.i

CMakeFiles/RayTracer.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracer.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine/Documents/Cours/M1/PPN_RayTracing/src/main.cpp -o CMakeFiles/RayTracer.dir/src/main.cpp.s

CMakeFiles/RayTracer.dir/src/camera.cpp.o: CMakeFiles/RayTracer.dir/flags.make
CMakeFiles/RayTracer.dir/src/camera.cpp.o: ../src/camera.cpp
CMakeFiles/RayTracer.dir/src/camera.cpp.o: CMakeFiles/RayTracer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine/Documents/Cours/M1/PPN_RayTracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/RayTracer.dir/src/camera.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracer.dir/src/camera.cpp.o -MF CMakeFiles/RayTracer.dir/src/camera.cpp.o.d -o CMakeFiles/RayTracer.dir/src/camera.cpp.o -c /home/amine/Documents/Cours/M1/PPN_RayTracing/src/camera.cpp

CMakeFiles/RayTracer.dir/src/camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracer.dir/src/camera.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine/Documents/Cours/M1/PPN_RayTracing/src/camera.cpp > CMakeFiles/RayTracer.dir/src/camera.cpp.i

CMakeFiles/RayTracer.dir/src/camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracer.dir/src/camera.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine/Documents/Cours/M1/PPN_RayTracing/src/camera.cpp -o CMakeFiles/RayTracer.dir/src/camera.cpp.s

CMakeFiles/RayTracer.dir/src/entity.cpp.o: CMakeFiles/RayTracer.dir/flags.make
CMakeFiles/RayTracer.dir/src/entity.cpp.o: ../src/entity.cpp
CMakeFiles/RayTracer.dir/src/entity.cpp.o: CMakeFiles/RayTracer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine/Documents/Cours/M1/PPN_RayTracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/RayTracer.dir/src/entity.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracer.dir/src/entity.cpp.o -MF CMakeFiles/RayTracer.dir/src/entity.cpp.o.d -o CMakeFiles/RayTracer.dir/src/entity.cpp.o -c /home/amine/Documents/Cours/M1/PPN_RayTracing/src/entity.cpp

CMakeFiles/RayTracer.dir/src/entity.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracer.dir/src/entity.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine/Documents/Cours/M1/PPN_RayTracing/src/entity.cpp > CMakeFiles/RayTracer.dir/src/entity.cpp.i

CMakeFiles/RayTracer.dir/src/entity.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracer.dir/src/entity.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine/Documents/Cours/M1/PPN_RayTracing/src/entity.cpp -o CMakeFiles/RayTracer.dir/src/entity.cpp.s

CMakeFiles/RayTracer.dir/src/position.cpp.o: CMakeFiles/RayTracer.dir/flags.make
CMakeFiles/RayTracer.dir/src/position.cpp.o: ../src/position.cpp
CMakeFiles/RayTracer.dir/src/position.cpp.o: CMakeFiles/RayTracer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine/Documents/Cours/M1/PPN_RayTracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/RayTracer.dir/src/position.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracer.dir/src/position.cpp.o -MF CMakeFiles/RayTracer.dir/src/position.cpp.o.d -o CMakeFiles/RayTracer.dir/src/position.cpp.o -c /home/amine/Documents/Cours/M1/PPN_RayTracing/src/position.cpp

CMakeFiles/RayTracer.dir/src/position.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracer.dir/src/position.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine/Documents/Cours/M1/PPN_RayTracing/src/position.cpp > CMakeFiles/RayTracer.dir/src/position.cpp.i

CMakeFiles/RayTracer.dir/src/position.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracer.dir/src/position.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine/Documents/Cours/M1/PPN_RayTracing/src/position.cpp -o CMakeFiles/RayTracer.dir/src/position.cpp.s

CMakeFiles/RayTracer.dir/src/ray.cpp.o: CMakeFiles/RayTracer.dir/flags.make
CMakeFiles/RayTracer.dir/src/ray.cpp.o: ../src/ray.cpp
CMakeFiles/RayTracer.dir/src/ray.cpp.o: CMakeFiles/RayTracer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine/Documents/Cours/M1/PPN_RayTracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/RayTracer.dir/src/ray.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracer.dir/src/ray.cpp.o -MF CMakeFiles/RayTracer.dir/src/ray.cpp.o.d -o CMakeFiles/RayTracer.dir/src/ray.cpp.o -c /home/amine/Documents/Cours/M1/PPN_RayTracing/src/ray.cpp

CMakeFiles/RayTracer.dir/src/ray.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracer.dir/src/ray.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine/Documents/Cours/M1/PPN_RayTracing/src/ray.cpp > CMakeFiles/RayTracer.dir/src/ray.cpp.i

CMakeFiles/RayTracer.dir/src/ray.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracer.dir/src/ray.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine/Documents/Cours/M1/PPN_RayTracing/src/ray.cpp -o CMakeFiles/RayTracer.dir/src/ray.cpp.s

CMakeFiles/RayTracer.dir/src/renderer.cpp.o: CMakeFiles/RayTracer.dir/flags.make
CMakeFiles/RayTracer.dir/src/renderer.cpp.o: ../src/renderer.cpp
CMakeFiles/RayTracer.dir/src/renderer.cpp.o: CMakeFiles/RayTracer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine/Documents/Cours/M1/PPN_RayTracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/RayTracer.dir/src/renderer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracer.dir/src/renderer.cpp.o -MF CMakeFiles/RayTracer.dir/src/renderer.cpp.o.d -o CMakeFiles/RayTracer.dir/src/renderer.cpp.o -c /home/amine/Documents/Cours/M1/PPN_RayTracing/src/renderer.cpp

CMakeFiles/RayTracer.dir/src/renderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracer.dir/src/renderer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine/Documents/Cours/M1/PPN_RayTracing/src/renderer.cpp > CMakeFiles/RayTracer.dir/src/renderer.cpp.i

CMakeFiles/RayTracer.dir/src/renderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracer.dir/src/renderer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine/Documents/Cours/M1/PPN_RayTracing/src/renderer.cpp -o CMakeFiles/RayTracer.dir/src/renderer.cpp.s

CMakeFiles/RayTracer.dir/src/scene.cpp.o: CMakeFiles/RayTracer.dir/flags.make
CMakeFiles/RayTracer.dir/src/scene.cpp.o: ../src/scene.cpp
CMakeFiles/RayTracer.dir/src/scene.cpp.o: CMakeFiles/RayTracer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine/Documents/Cours/M1/PPN_RayTracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/RayTracer.dir/src/scene.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracer.dir/src/scene.cpp.o -MF CMakeFiles/RayTracer.dir/src/scene.cpp.o.d -o CMakeFiles/RayTracer.dir/src/scene.cpp.o -c /home/amine/Documents/Cours/M1/PPN_RayTracing/src/scene.cpp

CMakeFiles/RayTracer.dir/src/scene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracer.dir/src/scene.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine/Documents/Cours/M1/PPN_RayTracing/src/scene.cpp > CMakeFiles/RayTracer.dir/src/scene.cpp.i

CMakeFiles/RayTracer.dir/src/scene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracer.dir/src/scene.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine/Documents/Cours/M1/PPN_RayTracing/src/scene.cpp -o CMakeFiles/RayTracer.dir/src/scene.cpp.s

CMakeFiles/RayTracer.dir/shapes/object.cpp.o: CMakeFiles/RayTracer.dir/flags.make
CMakeFiles/RayTracer.dir/shapes/object.cpp.o: ../shapes/object.cpp
CMakeFiles/RayTracer.dir/shapes/object.cpp.o: CMakeFiles/RayTracer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine/Documents/Cours/M1/PPN_RayTracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/RayTracer.dir/shapes/object.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracer.dir/shapes/object.cpp.o -MF CMakeFiles/RayTracer.dir/shapes/object.cpp.o.d -o CMakeFiles/RayTracer.dir/shapes/object.cpp.o -c /home/amine/Documents/Cours/M1/PPN_RayTracing/shapes/object.cpp

CMakeFiles/RayTracer.dir/shapes/object.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracer.dir/shapes/object.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine/Documents/Cours/M1/PPN_RayTracing/shapes/object.cpp > CMakeFiles/RayTracer.dir/shapes/object.cpp.i

CMakeFiles/RayTracer.dir/shapes/object.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracer.dir/shapes/object.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine/Documents/Cours/M1/PPN_RayTracing/shapes/object.cpp -o CMakeFiles/RayTracer.dir/shapes/object.cpp.s

CMakeFiles/RayTracer.dir/shapes/sphere.cpp.o: CMakeFiles/RayTracer.dir/flags.make
CMakeFiles/RayTracer.dir/shapes/sphere.cpp.o: ../shapes/sphere.cpp
CMakeFiles/RayTracer.dir/shapes/sphere.cpp.o: CMakeFiles/RayTracer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine/Documents/Cours/M1/PPN_RayTracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/RayTracer.dir/shapes/sphere.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracer.dir/shapes/sphere.cpp.o -MF CMakeFiles/RayTracer.dir/shapes/sphere.cpp.o.d -o CMakeFiles/RayTracer.dir/shapes/sphere.cpp.o -c /home/amine/Documents/Cours/M1/PPN_RayTracing/shapes/sphere.cpp

CMakeFiles/RayTracer.dir/shapes/sphere.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracer.dir/shapes/sphere.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine/Documents/Cours/M1/PPN_RayTracing/shapes/sphere.cpp > CMakeFiles/RayTracer.dir/shapes/sphere.cpp.i

CMakeFiles/RayTracer.dir/shapes/sphere.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracer.dir/shapes/sphere.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine/Documents/Cours/M1/PPN_RayTracing/shapes/sphere.cpp -o CMakeFiles/RayTracer.dir/shapes/sphere.cpp.s

CMakeFiles/RayTracer.dir/shapes/cube.cpp.o: CMakeFiles/RayTracer.dir/flags.make
CMakeFiles/RayTracer.dir/shapes/cube.cpp.o: ../shapes/cube.cpp
CMakeFiles/RayTracer.dir/shapes/cube.cpp.o: CMakeFiles/RayTracer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine/Documents/Cours/M1/PPN_RayTracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/RayTracer.dir/shapes/cube.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracer.dir/shapes/cube.cpp.o -MF CMakeFiles/RayTracer.dir/shapes/cube.cpp.o.d -o CMakeFiles/RayTracer.dir/shapes/cube.cpp.o -c /home/amine/Documents/Cours/M1/PPN_RayTracing/shapes/cube.cpp

CMakeFiles/RayTracer.dir/shapes/cube.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracer.dir/shapes/cube.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amine/Documents/Cours/M1/PPN_RayTracing/shapes/cube.cpp > CMakeFiles/RayTracer.dir/shapes/cube.cpp.i

CMakeFiles/RayTracer.dir/shapes/cube.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracer.dir/shapes/cube.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amine/Documents/Cours/M1/PPN_RayTracing/shapes/cube.cpp -o CMakeFiles/RayTracer.dir/shapes/cube.cpp.s

CMakeFiles/RayTracer.dir/ppm_writer/ppm_writer.c.o: CMakeFiles/RayTracer.dir/flags.make
CMakeFiles/RayTracer.dir/ppm_writer/ppm_writer.c.o: ../ppm_writer/ppm_writer.c
CMakeFiles/RayTracer.dir/ppm_writer/ppm_writer.c.o: CMakeFiles/RayTracer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amine/Documents/Cours/M1/PPN_RayTracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object CMakeFiles/RayTracer.dir/ppm_writer/ppm_writer.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/RayTracer.dir/ppm_writer/ppm_writer.c.o -MF CMakeFiles/RayTracer.dir/ppm_writer/ppm_writer.c.o.d -o CMakeFiles/RayTracer.dir/ppm_writer/ppm_writer.c.o -c /home/amine/Documents/Cours/M1/PPN_RayTracing/ppm_writer/ppm_writer.c

CMakeFiles/RayTracer.dir/ppm_writer/ppm_writer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RayTracer.dir/ppm_writer/ppm_writer.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/amine/Documents/Cours/M1/PPN_RayTracing/ppm_writer/ppm_writer.c > CMakeFiles/RayTracer.dir/ppm_writer/ppm_writer.c.i

CMakeFiles/RayTracer.dir/ppm_writer/ppm_writer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RayTracer.dir/ppm_writer/ppm_writer.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/amine/Documents/Cours/M1/PPN_RayTracing/ppm_writer/ppm_writer.c -o CMakeFiles/RayTracer.dir/ppm_writer/ppm_writer.c.s

# Object files for target RayTracer
RayTracer_OBJECTS = \
"CMakeFiles/RayTracer.dir/src/main.cpp.o" \
"CMakeFiles/RayTracer.dir/src/camera.cpp.o" \
"CMakeFiles/RayTracer.dir/src/entity.cpp.o" \
"CMakeFiles/RayTracer.dir/src/position.cpp.o" \
"CMakeFiles/RayTracer.dir/src/ray.cpp.o" \
"CMakeFiles/RayTracer.dir/src/renderer.cpp.o" \
"CMakeFiles/RayTracer.dir/src/scene.cpp.o" \
"CMakeFiles/RayTracer.dir/shapes/object.cpp.o" \
"CMakeFiles/RayTracer.dir/shapes/sphere.cpp.o" \
"CMakeFiles/RayTracer.dir/shapes/cube.cpp.o" \
"CMakeFiles/RayTracer.dir/ppm_writer/ppm_writer.c.o"

# External object files for target RayTracer
RayTracer_EXTERNAL_OBJECTS =

RayTracer: CMakeFiles/RayTracer.dir/src/main.cpp.o
RayTracer: CMakeFiles/RayTracer.dir/src/camera.cpp.o
RayTracer: CMakeFiles/RayTracer.dir/src/entity.cpp.o
RayTracer: CMakeFiles/RayTracer.dir/src/position.cpp.o
RayTracer: CMakeFiles/RayTracer.dir/src/ray.cpp.o
RayTracer: CMakeFiles/RayTracer.dir/src/renderer.cpp.o
RayTracer: CMakeFiles/RayTracer.dir/src/scene.cpp.o
RayTracer: CMakeFiles/RayTracer.dir/shapes/object.cpp.o
RayTracer: CMakeFiles/RayTracer.dir/shapes/sphere.cpp.o
RayTracer: CMakeFiles/RayTracer.dir/shapes/cube.cpp.o
RayTracer: CMakeFiles/RayTracer.dir/ppm_writer/ppm_writer.c.o
RayTracer: CMakeFiles/RayTracer.dir/build.make
RayTracer: CMakeFiles/RayTracer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/amine/Documents/Cours/M1/PPN_RayTracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable RayTracer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RayTracer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RayTracer.dir/build: RayTracer
.PHONY : CMakeFiles/RayTracer.dir/build

CMakeFiles/RayTracer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RayTracer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RayTracer.dir/clean

CMakeFiles/RayTracer.dir/depend:
	cd /home/amine/Documents/Cours/M1/PPN_RayTracing/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/amine/Documents/Cours/M1/PPN_RayTracing /home/amine/Documents/Cours/M1/PPN_RayTracing /home/amine/Documents/Cours/M1/PPN_RayTracing/build /home/amine/Documents/Cours/M1/PPN_RayTracing/build /home/amine/Documents/Cours/M1/PPN_RayTracing/build/CMakeFiles/RayTracer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RayTracer.dir/depend

