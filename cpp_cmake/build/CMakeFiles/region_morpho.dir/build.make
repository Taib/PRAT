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
CMAKE_SOURCE_DIR = /home/tramx/M2IMA/PRAT/cpp_cmake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tramx/M2IMA/PRAT/cpp_cmake/build

# Include any dependencies generated for this target.
include CMakeFiles/region_morpho.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/region_morpho.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/region_morpho.dir/flags.make

CMakeFiles/region_morpho.dir/region_morpho.cpp.o: CMakeFiles/region_morpho.dir/flags.make
CMakeFiles/region_morpho.dir/region_morpho.cpp.o: ../region_morpho.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tramx/M2IMA/PRAT/cpp_cmake/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/region_morpho.dir/region_morpho.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/region_morpho.dir/region_morpho.cpp.o -c /home/tramx/M2IMA/PRAT/cpp_cmake/region_morpho.cpp

CMakeFiles/region_morpho.dir/region_morpho.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/region_morpho.dir/region_morpho.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/tramx/M2IMA/PRAT/cpp_cmake/region_morpho.cpp > CMakeFiles/region_morpho.dir/region_morpho.cpp.i

CMakeFiles/region_morpho.dir/region_morpho.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/region_morpho.dir/region_morpho.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/tramx/M2IMA/PRAT/cpp_cmake/region_morpho.cpp -o CMakeFiles/region_morpho.dir/region_morpho.cpp.s

CMakeFiles/region_morpho.dir/region_morpho.cpp.o.requires:
.PHONY : CMakeFiles/region_morpho.dir/region_morpho.cpp.o.requires

CMakeFiles/region_morpho.dir/region_morpho.cpp.o.provides: CMakeFiles/region_morpho.dir/region_morpho.cpp.o.requires
	$(MAKE) -f CMakeFiles/region_morpho.dir/build.make CMakeFiles/region_morpho.dir/region_morpho.cpp.o.provides.build
.PHONY : CMakeFiles/region_morpho.dir/region_morpho.cpp.o.provides

CMakeFiles/region_morpho.dir/region_morpho.cpp.o.provides.build: CMakeFiles/region_morpho.dir/region_morpho.cpp.o

# Object files for target region_morpho
region_morpho_OBJECTS = \
"CMakeFiles/region_morpho.dir/region_morpho.cpp.o"

# External object files for target region_morpho
region_morpho_EXTERNAL_OBJECTS =

region_morpho: CMakeFiles/region_morpho.dir/region_morpho.cpp.o
region_morpho: CMakeFiles/region_morpho.dir/build.make
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_video.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_ts.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_superres.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_ocl.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_legacy.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_gpu.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_contrib.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_legacy.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_video.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.2.4.8
region_morpho: /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.8
region_morpho: CMakeFiles/region_morpho.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable region_morpho"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/region_morpho.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/region_morpho.dir/build: region_morpho
.PHONY : CMakeFiles/region_morpho.dir/build

CMakeFiles/region_morpho.dir/requires: CMakeFiles/region_morpho.dir/region_morpho.cpp.o.requires
.PHONY : CMakeFiles/region_morpho.dir/requires

CMakeFiles/region_morpho.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/region_morpho.dir/cmake_clean.cmake
.PHONY : CMakeFiles/region_morpho.dir/clean

CMakeFiles/region_morpho.dir/depend:
	cd /home/tramx/M2IMA/PRAT/cpp_cmake/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tramx/M2IMA/PRAT/cpp_cmake /home/tramx/M2IMA/PRAT/cpp_cmake /home/tramx/M2IMA/PRAT/cpp_cmake/build /home/tramx/M2IMA/PRAT/cpp_cmake/build /home/tramx/M2IMA/PRAT/cpp_cmake/build/CMakeFiles/region_morpho.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/region_morpho.dir/depend
