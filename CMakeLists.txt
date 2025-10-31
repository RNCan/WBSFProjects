cmake_minimum_required(VERSION 3.25)
project(WBSFProjects LANGUAGES CXX)

# Optionally enforce C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DMUPARSER_STATIC)

IF(MSVC)
    # Add /EHsc to the C++ compiler flags
    # It's important to append it to existing flags, not overwrite them.
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc") 
ENDIF(MSVC)

# Add -fPIC on Linux/UNIX
if(UNIX AND NOT APPLE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Add subdirectories (order matters: core first)
add_subdirectory(WBSF)
add_subdirectory(WBSFModels)
add_subdirectory(WBSFTools)
