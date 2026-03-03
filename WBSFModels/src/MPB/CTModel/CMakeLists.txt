cmake_minimum_required(VERSION 3.25)

######################  MPB-ColdTolerance  ######################

project(MPB-ColdTolerance)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES MPB-CTModel.h MPB-CTModel.cpp )

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as dynamic library
add_library(MPB-ColdTolerance SHARED
  ${SOURCE_FILES}
)

target_link_libraries(MPB-ColdTolerance PUBLIC
	MPBLib
)




