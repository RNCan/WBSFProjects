cmake_minimum_required(VERSION 3.25)
project(WBSFProjects LANGUAGES CXX)

# Optionally enforce C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DMUPARSER_STATIC)

# Add subdirectories (order matters: core first)
add_subdirectory(WBSF)
add_subdirectory(WBSFModels)
add_subdirectory(WBSFTools)
