cmake_minimum_required(VERSION 3.12)

project(GrabCutEdgeDetection)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

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

# Create the DLL
add_library(GrabCutEdgeDetection SHARED
    GrabCutEdgeDetection.cpp
)

# Link OpenCV libraries
target_link_libraries(GrabCutEdgeDetection PRIVATE ${OpenCV_LIBS})

# Set properties for the DLL
set_target_properties(GrabCutEdgeDetection PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS YES
)
