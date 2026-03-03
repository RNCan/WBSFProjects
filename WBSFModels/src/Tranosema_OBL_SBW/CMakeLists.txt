cmake_minimum_required(VERSION 3.25)

project(Tranosema_OBL_SBW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES Tranosema_OBL_SBW.h Tranosema_OBL_SBW.cpp Tranosema_OBL_SBW_Model.h Tranosema_OBL_SBW_Model.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Build as shared library
add_library(Tranosema_OBL_SBW SHARED
  ${SOURCE_FILES}
)

target_link_libraries(Tranosema_OBL_SBW PUBLIC
	TranosemaLib
	ObliqueBandedLeafrollerLib
	SpruceBudwormLib
)

