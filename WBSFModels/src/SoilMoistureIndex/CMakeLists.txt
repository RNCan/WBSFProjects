######################  SoilMoistureIndex  ######################

project(SoilMoistureIndex)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES SMIModel.h SMIModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(SoilMoistureIndex SHARED
  ${SOURCE_FILES}
)

target_link_libraries(SoilMoistureIndex PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)

######################  SoilMoistureIndexQL  ######################

project(SoilMoistureIndexQL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES SMI_QLModel.h SMI_QLModel.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(SoilMoistureIndexQL SHARED
  ${SOURCE_FILES}
)

target_link_libraries(SoilMoistureIndexQL PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)