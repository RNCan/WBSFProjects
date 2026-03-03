project(AprocerosLeucopoda)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES ALeucopoda.h ALeucopoda.cpp ALeucopodaEquations.h ALeucopodaEquations.cpp ALeucopodaModel.h ALeucopodaModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_library(AprocerosLeucopoda SHARED  
  ${SOURCE_FILES}  
)

target_link_libraries(AprocerosLeucopoda PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased	
)


