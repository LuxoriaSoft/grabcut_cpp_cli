cmake_minimum_required(VERSION 3.15)
project(GrabCutInterop)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the build type to Release
set(CMAKE_BUILD_TYPE Release)

# Set the OpenCV static flag
set(OpenCV_STATIC ON)

# Handle OpenCV on different platforms
if(WIN32)
    set(OpenCV_DIR "D:/opencv-install")
    find_package(OpenCV REQUIRED PATHS "D:/opencv-install" NO_DEFAULT_PATH)
elseif(APPLE)
    # If on macOS, Homebrew might be used to install OpenCV
    find_package(OpenCV REQUIRED PATHS /usr/local/opt/opencv4 /opt/homebrew/opt/opencv4)
else()
    # Default OpenCV search if the platform is neither macOS nor Windows
    find_package(OpenCV REQUIRED)
endif()

# Ensure that OpenCV is found
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found. Please install OpenCV and make sure it's in the CMake search path.")
endif()

# Specify the location of the source files
set(SOURCE_FILES
        GrabCutAlgorithm.cpp
        GrabCutAlgorithm.hpp
)

# Create the static library (uncomment the shared version if you need a DLL)
add_library(GrabCutAlgorithm STATIC ${SOURCE_FILES})

# Include directories for headers
target_include_directories(GrabCutAlgorithm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link OpenCV libraries
target_link_libraries(grabcut_edge_detection ${OpenCV_LIBS})

# Optional: Add include directories for OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})
