cmake_minimum_required(VERSION 3.25)

project(MeteorusTrachynotusLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
file(GLOB SOURCE_FILES MeteorusTrachynotus.h MeteorusTrachynotus.cpp MeteorusTrachynotusEquations.h MeteorusTrachynotusEquations.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as dynamic library
add_library(MeteorusTrachynotusLib STATIC
  ${SOURCE_FILES}  
)

target_link_libraries(MeteorusTrachynotusLib PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  MeteorusTrachynotusBiology  ######################

project(MeteorusTrachynotusBiology)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES MeteorusTrachynotusModel.h MeteorusTrachynotusModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(MeteorusTrachynotusBiology SHARED
  ${SOURCE_FILES}
)

target_link_libraries(MeteorusTrachynotusBiology PUBLIC
	MeteorusTrachynotusLib
)