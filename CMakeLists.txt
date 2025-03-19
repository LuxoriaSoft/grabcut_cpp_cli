cmake_minimum_required(VERSION 3.15)
project(GrabCutInterop)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the location of the source files
set(SOURCE_FILES
        GrabCutAlgorithm.cpp
        GrabCutAlgorithm.hpp
)

# Create the static library (uncomment the shared version if you need a DLL)
add_library(GrabCutAlgorithm STATIC ${SOURCE_FILES})

# Include directories for headers
target_include_directories(GrabCutAlgorithm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Optionally, if you want a DLL, you can replace the line above with:
# add_library(GrabCutAlgorithm SHARED ${SOURCE_FILES})
