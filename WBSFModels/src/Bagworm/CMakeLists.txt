project(Bagworm)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES Bagworm.h Bagworm.cpp BagwormModel.h BagwormModel.cpp resource.h)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}     
)

# Build as dynamic library
add_library(Bagworm SHARED  
  ${SOURCE_FILES}  
)

target_link_libraries(Bagworm PUBLIC
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased	
)


