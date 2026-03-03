cmake_minimum_required(VERSION 3.25)

project(FallCankerworms)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set (SOURCE_FILES FallCankerwormsModel.h FallCankerwormsModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_library(FallCankerworms SHARED
  ${SOURCE_FILES}
)

target_link_libraries(FallCankerworms PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased	
)

######################  SpringCankerworms  ######################

project(SpringCankerworms)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set (SOURCE_FILES SpringCankerwormsModel.h SpringCankerwormsModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_library(SpringCankerworms SHARED
  ${SOURCE_FILES}
)

target_link_libraries(SpringCankerworms PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased	
)

