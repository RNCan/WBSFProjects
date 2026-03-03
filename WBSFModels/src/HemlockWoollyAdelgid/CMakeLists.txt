cmake_minimum_required(VERSION 3.25)

project(HemlockWoollyAdelgidColdTolerance)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES HemlockWoollyAdelgid.h HemlockWoollyAdelgid.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_library(HemlockWoollyAdelgidColdTolerance SHARED
  ${SOURCE_FILES}
)

target_link_libraries(HemlockWoollyAdelgidColdTolerance PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased	
)

######################  HWAPhenology  ######################

project(HemlockWoollyAdelgidPhenology)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES HWAPhenology.h HWAPhenology.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_library(HemlockWoollyAdelgidPhenology SHARED
  ${SOURCE_FILES}
)

target_link_libraries(HemlockWoollyAdelgidPhenology PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased	
)

