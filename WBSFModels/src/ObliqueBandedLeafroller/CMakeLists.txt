cmake_minimum_required(VERSION 3.25)

project(ObliqueBandedLeafrollerLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set (SOURCE_FILES ObliqueBandedLeafroller.h ObliqueBandedLeafroller.cpp ObliqueBandedLeafrollerEquations.h ObliqueBandedLeafrollerEquations.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

add_library(ObliqueBandedLeafrollerLib STATIC
  ${SOURCE_FILES}
)

target_link_libraries(ObliqueBandedLeafrollerLib PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

project(ObliqueBandedLeafroller)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set (SOURCE_FILES ObliqueBandedLeafrollerModel.h ObliqueBandedLeafrollerModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as dynamic library
add_library(ObliqueBandedLeafroller SHARED
    ${SOURCE_FILES}
)

target_link_libraries(ObliqueBandedLeafroller PUBLIC
	ObliqueBandedLeafrollerLib
)


