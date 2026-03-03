cmake_minimum_required(VERSION 3.25)

project(TranosemaLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES Tranosema.h Tranosema.cpp TranosemaEquations.h TranosemaEquations.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as static library
add_library(TranosemaLib STATIC
  ${SOURCE_FILES}
)

target_link_libraries(TranosemaLib PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  TranosemaBiology  ######################

project(TranosemaBiology)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES TranosemaModel.h TranosemaModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}  
)

# Build as shared library
add_library(TranosemaBiology SHARED
  ${SOURCE_FILES}
)

target_link_libraries(TranosemaBiology PUBLIC
	TranosemaLib
)


