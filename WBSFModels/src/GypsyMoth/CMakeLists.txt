cmake_minimum_required(VERSION 3.25)

project(GypsyMothLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES EggModel.h EggModel.cpp 
                 GrayModel.h GrayModel.cpp 
				 GypsyMoth.h GypsyMoth.cpp
				 GypsyMothCommon.h GypsyMothCommon.cpp
				 JohnsonModel.cpp JohnsonModel.h 
				 LyonsModel.cpp LyonsModel.h
				 SawyerModel.cpp SawyerModel.h
)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as static library
add_library(GypsyMothLib STATIC
  ${SOURCE_FILES}
)

target_link_libraries(GypsyMothLib PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased	
)

######################  GypsyMothSeasonality  ######################

project(GypsyMothSeasonality)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES GypsyMothModel.h GypsyMothModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_library(GypsyMothSeasonality SHARED
  ${SOURCE_FILES}
)

target_link_libraries(GypsyMothSeasonality PUBLIC
	GypsyMothLib
)

######################  GypsyMothStability  ######################

project(GypsyMothStability)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES GypsyMothStab.h GypsyMothStab.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_library(GypsyMothStability SHARED
  ${SOURCE_FILES}  
)

target_link_libraries(GypsyMothStability PUBLIC
	GypsyMothLib
)