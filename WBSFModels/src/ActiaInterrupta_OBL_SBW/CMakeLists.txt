cmake_minimum_required(VERSION 3.25)

######################  ActiaInterrupta_OBL_SBW  ######################

project(ActiaInterrupta_OBL_SBW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES ActiaInterrupta_OBL_SBW.h ActiaInterrupta_OBL_SBW.cpp ActiaInterrupta_OBL_SBW_Model.h ActiaInterrupta_OBL_SBW_Model.cpp )

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Build as dynamic library
add_library(ActiaInterrupta_OBL_SBW SHARED  
  ${SOURCE_FILES}  
)

target_link_libraries(ActiaInterrupta_OBL_SBW PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
	ActiaInterruptaLib
	ObliqueBandedLeafrollerLib
	SpruceBudwormLib
)
