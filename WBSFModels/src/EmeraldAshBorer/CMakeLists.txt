cmake_minimum_required(VERSION 3.25)

project(EmeraldAshBorer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set (SOURCE_FILES EmeraldAshBorer.h EmeraldAshBorer.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_library(EmeraldAshBorer SHARED
  ${SOURCE_FILES}
)

target_link_libraries(EmeraldAshBorer PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased	
)

######################  EmeraldAshBorerColdHardiness  ######################

project(EmeraldAshBorerColdHardiness)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set (SOURCE_FILES EmeraldAshBorerColdHardiness.h EmeraldAshBorerColdHardiness.cpp TreeMicroClimate.h TreeMicroClimate.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_library(EmeraldAshBorerColdHardiness SHARED
  ${SOURCE_FILES}
)

target_link_libraries(EmeraldAshBorerColdHardiness PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased	
)