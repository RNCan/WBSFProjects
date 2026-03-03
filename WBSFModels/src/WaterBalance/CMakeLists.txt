######################  WaterBalance  ######################

project(WaterBalance)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES WaterBalance.h WaterBalance.cpp McCabeWBM.h McCabeWBM.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(WaterBalance SHARED
  ${SOURCE_FILES}
)

target_link_libraries(WaterBalance PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)
