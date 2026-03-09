######################  StdPrcpETIndex  ######################

project(StdPrcpETIndex)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the GLOB command to get all the source files
set(SOURCE_FILES SPEIModel.h SPEIModel.cpp spei.h spei.cpp pdfs.h pdfs.cpp lmoments.h lmoments.cpp auxiliary.h auxiliary.cpp Thornthwaite.h Thornthwaite.cpp)

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}    
)

# Build as dynamic library
add_library(StdPrcpETIndex SHARED
  ${SOURCE_FILES}
)

target_link_libraries(StdPrcpETIndex PUBLIC
	BioSIM_API
	WBSFBasic
	WBSFModelBased
	WBSFWeatherBased
)
