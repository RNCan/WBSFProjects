cmake_minimum_required(VERSION 3.25)

project(ActiaInterruptaLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES ActiaInterrupta.h ActiaInterrupta.cpp ActiaInterruptaEquations.h ActiaInterruptaEquations.cpp)

# Build as static library
add_library(ActiaInterruptaLib STATIC
  ${SOURCE_FILES}
)

target_include_directories(ActiaInterruptaLib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

target_link_libraries(ActiaInterruptaLib PUBLIC
	WBSFBasic
)

######################  ActiaInterruptaBiology  ######################

project(ActiaInterruptaBiology)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES ActiaInterruptaModel.h ActiaInterruptaModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}   
)

# Build as dynamic library
add_library(ActiaInterruptaBiology SHARED  
  ${SOURCE_FILES}  
)

target_link_libraries(ActiaInterruptaBiology PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
	ActiaInterruptaLib
)
