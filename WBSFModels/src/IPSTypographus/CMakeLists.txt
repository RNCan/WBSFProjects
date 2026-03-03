cmake_minimum_required(VERSION 3.25)

project(SpruceBarkBeetleLiterature)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
file(GLOB SOURCE_FILES ./SpruceBarkBeetleLiteratureModel.h ./SpruceBarkBeetleLiteratureModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_library(SpruceBarkBeetleLiterature SHARED
  ${SOURCE_FILES}
)

target_link_libraries(SpruceBarkBeetleLiterature PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)


