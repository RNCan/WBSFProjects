cmake_minimum_required(VERSION 3.25)

######################  MPB-SLR  ######################

project(MPB-SLR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES SLR.h SLR.cpp MPB-SLRModel.h MPB-SLRModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as dynamic library
add_library(MPB-SLR SHARED
  ${SOURCE_FILES}
)

target_link_libraries(MPB-SLR PUBLIC
	MPBLib
)
