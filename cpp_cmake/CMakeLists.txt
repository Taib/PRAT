# cmake needs this line
cmake_minimum_required(VERSION 2.8)

# Define project name
project(opencv_example_project)

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

# Add OpenCV headers location to your include paths
include_directories(${OpenCV_INCLUDE_DIRS})

# Declare the executable target built from your sources
#add_executable(main main.cpp)

add_executable(pixel_denoise pixel_denoise.cpp)
add_executable(pixel_morpho pixel_morpho.cpp)
add_executable(pixel_segmentation pixel_segmentation.cpp)
add_executable(region_morpho region_morpho.cpp)
add_executable(region_segmentation region_segmentation.cpp)

# Link your application with OpenCV libraries
#target_link_libraries(main ${OpenCV_LIBS})
target_link_libraries(pixel_denoise ${OpenCV_LIBS})
target_link_libraries(pixel_morpho ${OpenCV_LIBS})
target_link_libraries(pixel_segmentation ${OpenCV_LIBS})
target_link_libraries(region_morpho ${OpenCV_LIBS})
target_link_libraries(region_segmentation ${OpenCV_LIBS})
