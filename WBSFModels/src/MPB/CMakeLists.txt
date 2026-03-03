cmake_minimum_required(VERSION 3.25)

project(ClimaticMPB)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES ClimaticMPB.h ClimaticMPB.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(ClimaticMPB SHARED
  ${SOURCE_FILES}
)

target_link_libraries(ClimaticMPB PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

project(MPBLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES MPBColdTolerance.h MPBColdTolerance.cpp MPBDevRates.h MPBDevRates.cpp DevelopementVector.h DevelopementVector.cpp Generations.h Generations.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as static library
add_library(MPBLib STATIC
  ${SOURCE_FILES}
)

target_link_libraries(MPBLib PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

add_subdirectory(CTModel)
add_subdirectory(iModel)
add_subdirectory(SLRModel)

