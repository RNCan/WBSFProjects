cmake_minimum_required(VERSION 3.25)

######################  SiteIndexClimate  ######################

project(SiteIndexClimate)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES SiteIndexClimate.h SiteIndexClimate.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(SiteIndexClimate SHARED
  ${SOURCE_FILES}
)

target_link_libraries(SiteIndexClimate PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)